#pragma once

#include "memory_operator.h"
#include <cassert>
#include <cstring>

namespace GB {
constexpr size_t ROM_DATA_LENGTH = 0x100;
class BootstrapROM final : public GB::MemoryOperator {
public:
  BootstrapROM(const byte *rom_data) {
    memcpy(rom_data_, rom_data, sizeof(rom_data_));
  }

  void set(a16_t addr, byte data) override { assert(0 /*read-only*/); }
  byte get(a16_t addr) const override {
    assert(addr < ROM_DATA_LENGTH);
    return rom_data_[addr];
  }

private:
  byte rom_data_[ROM_DATA_LENGTH];
};

} // namespace GB
