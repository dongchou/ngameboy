#include "cpu.h"
#include "instruction_decoder.h"
#include <cassert>
#include <cstring>

namespace GB {

std::string CPU::reg_name(CPURegister reg) {
  switch (reg) {
  case CPURegister::REG_A:
    return "A";
  case CPURegister::REG_F:
    return "F";
  case CPURegister::REG_B:
    return "B";
  case CPURegister::REG_C:
    return "C";
  case CPURegister::REG_D:
    return "D";
  case CPURegister::REG_E:
    return "E";
  case CPURegister::REG_H:
    return "H";
  case CPURegister::REG_L:
    return "L";
  case CPURegister::REG_SP:
    return "SP";
  case CPURegister::REG_PC:
    return "PC";
  case CPURegister::REG_AF:
    return "AF";
  case CPURegister::REG_BC:
    return "BC";
  case CPURegister::REG_DE:
    return "DE";
  case CPURegister::REG_HL:
    return "HL";
  default:
    return "unknown";
  }
}
std::string CPU::condition_name(JumpCC cc) {
  switch (cc) {
  case JumpCC::CC_NZ:
    return "NZ";
  case JumpCC::CC_NC:
    return "NC";
  case JumpCC::CC_Z:
    return "Z";
  case JumpCC::CC_C:
    return "C";
  default:
    return "unknown";
  }
}

CPU::CPU(Memory *memory, bool debug_mode)
    : debug_mode_(debug_mode), _memory(memory), double_speed_mode_(false),
      clock_frequency_(NORMAL_CLOCK_FREQUENCY), halted_(false), _ime(false),
      _interrupt_enable(0), _interrupt_flags(0), timer_(new Timer()) {
  memset(_registers, 0, sizeof(_registers));
  memory->map_port(MappedIOPorts::REG_DIV, timer_.get());
  memory->map_port(MappedIOPorts::REG_TIMA, timer_.get());
  memory->map_port(MappedIOPorts::REG_TMA, timer_.get());
  memory->map_port(MappedIOPorts::REG_TAC, timer_.get());
}

CPU::~CPU() {}

reg16_t CPU::reg(CPURegister r) const {
  if (REG_MIN <= r && r < REG_MAX) {
    return _registers[r];
  }
  switch (r) {
  case REG_AF:
    return (_registers[REG_A] << 8) | _registers[REG_F];
  case REG_BC:
    return (_registers[REG_B] << 8) | _registers[REG_C];
  case REG_DE:
    return (_registers[REG_D] << 8) | _registers[REG_E];
  case REG_HL:
    return (_registers[REG_H] << 8) | _registers[REG_L];
  default:
    assert(0);
  }
  return 0;
}

void CPU::reg(CPURegister r, reg16_t v) {
  switch (r) {
  case REG_A:
  case REG_F:
  case REG_B:
  case REG_C:
  case REG_D:
  case REG_E:
  case REG_H:
  case REG_L:
    _registers[r] = v & 0xff;
    break;
  case REG_SP:
  case REG_PC:
    _registers[r] = v;
    break;
  case REG_AF:
    _registers[REG_A] = v >> 8;
    _registers[REG_F] = v & 0xF0;
    break;
  case REG_BC:
    _registers[REG_B] = v >> 8;
    _registers[REG_C] = v & 0xFF;
    break;
  case REG_DE:
    _registers[REG_D] = v >> 8;
    _registers[REG_E] = v & 0xFF;
    break;
  case REG_HL:
    _registers[REG_H] = v >> 8;
    _registers[REG_L] = v & 0xFF;
    break;
  default:
    assert(0);
    break;
  }
}

byte CPU::memory(size_t index) const {
  switch (index) {
  case MappedIOPorts::REG_IE:
    return _interrupt_enable;
  case MappedIOPorts::REG_IF:
    return _interrupt_flags;
  }
  return _memory->get(index);
}

void CPU::memory(size_t index, byte v) {
  switch (index) {
  case MappedIOPorts::REG_IE:
    set_ie(v);
    return;
  case MappedIOPorts::REG_IF:
    set_if(v);
    return;
  }
  _memory->set(index, v);
}

reg16_t CPU::pop() {
  reg16_t addr = _registers[CPURegister::REG_SP];
  assert(uint32_t(addr) + 2 <= 0xFFFE);
  reg16_t v = _memory->get(addr + 1);
  v = (v << 8) | _memory->get(addr);
  _registers[CPURegister::REG_SP] = addr + 2;
  return v;
}

void CPU::push(reg16_t v) {
  reg16_t addr = _registers[CPURegister::REG_SP];
  assert(addr >= 2);
  addr -= 2;
  _memory->set(addr, v & 0xff);
  _memory->set(addr + 1, v >> 8);
  _registers[CPURegister::REG_SP] = addr;
}

void CPU::set_ie(uint8_t data) {
  assert(data < MAX_INTERRUPT_VALUE);
  _interrupt_enable = data;
}

void CPU::set_if(uint8_t data) {
  assert(data < MAX_INTERRUPT_VALUE);
  _interrupt_flags = data;
}

void CPU::handle_interrupts() {
  // if the [IME] is disabled and it's in the haled status,interrupt just wakeup
  // the halt status,and nothing else happens(flag will not be reset)
  if (!_ime && !halted_) {
    return;
  }

  constexpr a16_t int_vecs[] = {0x40, 0x48, 0x50, 0x58, 0x60};

  int i = CPUInterrupts::INT_V_BLANK;
  for (; i <= CPUInterrupts::INT_JOYPAD; ++i) {
    if (!IS_BIT_SET(_interrupt_enable, i)) {
      continue;
    }
    if (!IS_BIT_SET(_interrupt_flags, i)) {
      continue;
    }
    break;
  }

  if (i == CPUInterrupts::INT_NONE) {
    return; // then nothing happens
  }

  if (halted_) {
    halted_ = false;
  }

  if (!_ime) {
    debug_log(
        "resume from the halt status by the interrupt :%d,enable:%x,flag:%x", i,
        _interrupt_enable, _interrupt_flags);
    // TODO:dmg bug, skip the next instruction.
    return;
  }

  CLEAR_BIT(_interrupt_flags, i);
  _ime = false;

  push(reg(CPURegister::REG_PC));
  reg(CPURegister::REG_PC, int_vecs[i]);

  if (i != CPUInterrupts::INT_V_BLANK)
    debug_log("now handle interrupt :%d,enable:%x,flag:%x", i,
              _interrupt_enable, _interrupt_flags);
  return;
}

std::string CPU::debug_info() const {
  char buff[2048];
  snprintf(buff, sizeof(buff),
           "[AF:0x%04x][BC:0x%04x][DE:0x%04x][HL:0x%04x][SP:0x%04x][PC:0x%04x]",
           reg(CPURegister::REG_AF), reg(CPURegister::REG_BC),
           reg(CPURegister::REG_DE), reg(CPURegister::REG_HL),
           reg(CPURegister::REG_SP), reg(CPURegister::REG_PC));
  return std::string(buff);
}

int CPU::update() {
  if (halted_) {
    timer_->update(this, 4);
    handle_interrupts();
    return 4;
  }

  GB::InstructionDecoder decoder(this);
  std::string dbg_info;
  auto pc = reg(CPURegister::REG_PC);
  if (debug_mode_) {
    dbg_info = debug_info();
  }
  auto instruction = decoder.decode();
  if (debug_mode_) {
    printf("[0x%04x] %-20s %s\n", pc, instruction->name().c_str(),
           dbg_info.c_str());
  }
  auto cost_clocks = instruction->execute();
  timer_->update(this, cost_clocks);
  handle_interrupts();
  return cost_clocks;
}

} // namespace GB