#pragma once

#include "hardware.h"
#include "memory_operator.h"

namespace GB {
class CPU;

/*
    00: CPU Clock / 1024 (DMG, CGB:   4096 Hz, SGB:   ~4194 Hz)
    01: CPU Clock / 16   (DMG, CGB: 262144 Hz, SGB: ~268400 Hz)
    10: CPU Clock / 64   (DMG, CGB:  65536 Hz, SGB:  ~67110 Hz)
    11: CPU Clock / 256  (DMG, CGB:  16384 Hz, SGB:  ~16780 Hz)
*/
constexpr uint32_t TIMER_TIMA_MODES[] = {10 /*2^10=1024*/, 4, 6, 8};
class Timer final : public IPortOperator {
public:
  Timer();

  void set_reg(a16_t addr, byte data) override;
  byte get_reg(a16_t addr) const override;
  void update(CPU *cpu, int clocks);

private:
  uint16_t internal_counter_;
  uint8_t timer_tima_;
  uint16_t timer_tima_acc_clocks_;
  uint8_t timer_tma_;
  uint8_t timer_tac_;
};
} // namespace GB
