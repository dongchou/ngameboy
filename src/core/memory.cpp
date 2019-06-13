#include "memory.h"
#include <cstring>

namespace GB {
Memory::Memory()
    : gpu_(nullptr), oam_(nullptr), cartridge_(nullptr), boot_rom_(nullptr) {
  memset(_memory, 0, sizeof(_memory));
  memset(ioport_handlers, 0, sizeof(ioport_handlers));
}
Memory::~Memory() {}

byte Memory::get(a16_t addr) const {
  if (addr < 0x100 && boot_rom_ != nullptr) {
    return boot_rom_->get(addr);
  }
  if (addr < 0x8000) {
    return cartridge_->get(addr);
  } else if (addr < 0xa000) {
    return gpu_->get(addr);
  } else if (addr < 0xc000) {
    return cartridge_->get(addr);
  } else if (addr >= 0xE000 && addr < 0xFE00) {
    addr -= 0x2000; // echo
  } else if (addr >= 0xFE00 && addr < 0xFEA0) {
    return oam_->get(addr);
  } else if (addr >= 0xFF00) {
    auto opr = get_ioport_handle(addr);
    if (opr != nullptr) {
      return opr->get_reg(addr);
    }
  }
  return _memory[addr];
}

void Memory::set(a16_t addr, byte data) {
  if (addr < 0x100 && boot_rom_ != nullptr) {
    boot_rom_->set(addr, data);
    return;
  }
  if (addr < 0x8000) {
    cartridge_->set(addr, data);
    return;
  } else if (addr < 0xa000) {
    gpu_->set(addr, data);
    return;
  } else if (addr < 0xc000) {
    cartridge_->set(addr, data);
    return;
  } else if (addr >= 0xE000 && addr < 0xFE00) {
    addr -= 0x2000;
  } else if (addr >= 0xFE00 && addr < 0xFEA0) {
    oam_->set(addr, data);
    return;
  } else if (addr >= 0xFF00) {
    auto opr = get_ioport_handle(addr);
    if (opr != nullptr) {
      opr->set_reg(addr, data);
      return;
    }
  }
  _memory[addr] = data;
}

} // namespace GB