#pragma once

#include "hardware.h"
#include "memory_operator.h"
#include <map>

#include <cassert>

namespace GB {

/*
General Memory Map
  0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
  4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
  8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
  A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
  C000-CFFF   4KB Work RAM Bank 0 (WRAM)
  D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
  E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
  FE00-FE9F   Sprite Attribute Table (OAM)
  FEA0-FEFF   Not Usable
  FF00-FF7F   I/O Ports
  FF80-FFFE   High RAM (HRAM)
  FFFF        Interrupt Enable Register
*/

constexpr size_t MEMORY_ROOM = 0x10000;
constexpr size_t MAX_IO_PORT_NUM = 0x100;

class Memory final : public non_copyable {
public:
  Memory();
  ~Memory();

  byte get(a16_t addr) const;
  void set(a16_t addr, byte data);

  void connect_gpu(MemoryOperator *opr) { gpu_ = opr; }
  void connect_oam(MemoryOperator *opr) { oam_ = opr; }
  void load_cartridge(MemoryOperator *opr) { cartridge_ = opr; }
  void load_boot_rom(MemoryOperator *opr) { boot_rom_ = opr; }
  void unload_boot_rom() { boot_rom_ = nullptr; }

  IPortOperator *get_ioport_handle(a16_t addr) const {
    assert(addr >= 0xFF00);
    return ioport_handlers[addr - 0xFF00];
  }

  bool map_port(a16_t addr, IPortOperator *opr) {
    assert(addr >= 0xFF00);
    addr -= 0xFF00;
    if (ioport_handlers[addr] != nullptr) {
      return false;
    }
    ioport_handlers[addr] = opr;
    return true;
  }

private:
  void set_dma(d8_t v);

private:
  byte _memory[MEMORY_ROOM];
  IPortOperator *ioport_handlers[MAX_IO_PORT_NUM];
  MemoryOperator *gpu_;
  MemoryOperator *oam_;
  MemoryOperator *cartridge_;
  MemoryOperator *boot_rom_;
};

} // namespace GB