#pragma once

#include "instruction.h"

namespace GB {
//=========== miscellaneous instructions ================
class CCF_Inst : public Instruction {
public:
  CCF_Inst(CPU *cpu) : Instruction(cpu) {}

  int internal_exec() override {
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    auto c = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, !c);
    return 4;
  }
  std::string name() override { return std::string("CCF"); }
};

class CPL_Inst : public Instruction {
public:
  CPL_Inst(CPU *cpu) : Instruction(cpu) {}

  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    dest = ~dest;
    _cpu->reg(CPURegister::REG_A, dest);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, true);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, true);
    return 4;
  }
  std::string name() override { return std::string("CPL"); }
};

class DAA_Inst : public Instruction {
public:
  DAA_Inst(CPU *cpu) : Instruction(cpu) {}

  int internal_exec() override {
    int correction = 0;
    auto flag_n = _cpu->test_flag(CPUFlagReg::FLAG_SUB);
    auto flag_h = _cpu->test_flag(CPUFlagReg::FLAG_HALF_CARRY);
    auto flag_c = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);

    reg8_t value = _cpu->reg(CPURegister::REG_A);
    auto result_c = false;

    if (flag_h || (!flag_n && (value & 0xf) > 9)) {
      correction += 0x6;
    }

    if (flag_c || (!flag_n && value > 0x99)) {
      correction += 0x60;
      result_c = true;
    }

    value += flag_n ? -correction : correction;

    _cpu->reg(CPURegister::REG_A, value);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, value == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, result_c);

    return 4;
  }
  std::string name() override { return std::string("DAA"); }
};

// disable interrupts
class DI_Inst : public Instruction {
public:
  DI_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    _cpu->disable_all_interrupt();
    return 4;
  }
  std::string name() override { return std::string("DI"); }
};

// enable interrupts
class EI_Inst : public Instruction {
public:
  EI_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    _cpu->enable_all_interrupt();
    return 4;
  }
  std::string name() override { return std::string("EI"); }
};

class Halt_Inst : public Instruction {
public:
  Halt_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    _cpu->halt();
    return 4;
  }
  std::string name() override { return std::string("HALT"); }
};

class Nop_Inst : public Instruction {
public:
  Nop_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override { return 4; }
  std::string name() override { return std::string("NOP"); }
};

class Stop_Inst : public Instruction {
public:
  Stop_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override { return 4; }
  std::string name() override { return std::string("STOP"); }
};

class RST_Inst : public Instruction {
public:
  RST_Inst(CPU *cpu, vec_t vec) : Instruction(cpu), _vec(vec) {}

  int internal_exec() override {
    _cpu->push(_cpu->reg(CPURegister::REG_PC));
    _cpu->reg(CPURegister::REG_PC, _vec);
    return 16;
  }
  std::string name() override {
    std::ostringstream ss;
    ss << "RST " << std::uppercase << std::setw(2) << std::setfill('0')
       << std::hex << _vec << "H";
    return ss.str();
  }

protected:
  vec_t _vec;
};

} // namespace GB