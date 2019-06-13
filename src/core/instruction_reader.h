#pragma once

#include "cpu.h"
#include "hardware.h"

namespace GB {
class InstructionReader final : public non_copyable {
public:
  InstructionReader(CPU *cpu) : cpu_(cpu), pc_offset_(0) {}
  byte readByte() {
    auto pc = _cpu->reg(CPURegister::REG_PC);
    auto data = _cpu->memory(pc + pc_offset_);
    ++pc_offset_;
    return data;
  }
  bool remain(size_t n) const { return true; }
  int finish() const { pc_offset_ = 0; }

private:
  CPU *cpu_;
  int pc_offset_;
};

} // namespace GB