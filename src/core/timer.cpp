#include "timer.h"
#include "common.h"
#include "cpu.h"

namespace GB {

Timer::Timer()
    : internal_counter_(0), timer_tima_(0), timer_tima_acc_clocks_(0),
      timer_tma_(0), timer_tac_(0) {}

void Timer::set_reg(a16_t addr, byte data) {
  switch (addr) {
  case MappedIOPorts::REG_DIV:
    // anything written to div will cause it to be reset to zero
    internal_counter_ = 0;
    break;
  case MappedIOPorts::REG_TIMA:
    timer_tima_ = data;
    break;
  case MappedIOPorts::REG_TMA:
    timer_tma_ = data;
    break;
  case MappedIOPorts::REG_TAC:
    timer_tac_ = data;
    break;
  default:
    assert(0);
  }
}

byte Timer::get_reg(a16_t addr) const {
  switch (addr) {
  case MappedIOPorts::REG_DIV:
    return static_cast<byte>(internal_counter_ >> 8);
  case MappedIOPorts::REG_TIMA:
    return timer_tima_;
  case MappedIOPorts::REG_TMA:
    return timer_tma_;
  case MappedIOPorts::REG_TAC:
    return timer_tac_;
  default:
    assert(0);
    return 0;
  }
}

void Timer::update(CPU *cpu, int clocks) {
  internal_counter_ += clocks;
  auto timer_enable = IS_BIT_SET(timer_tac_, 2);
  if (!timer_enable) {
    return;
  }
  //   debug_log(
  //       "====internal:%04x,timer_tima_acc_clocks_:%02x,timer_tima_:%02x,timer_"
  //       "tac_:%02x,timer_tac_:%02x==========",
  //       internal_counter_, timer_tima_acc_clocks_, timer_tima_, timer_tac_,
  //       timer_tac_ & 0x3);
  auto related_speed = TIMER_TIMA_MODES[timer_tac_ & 0x3];
  timer_tima_acc_clocks_ += clocks;

  uint16_t overflow = timer_tima_acc_clocks_ >> related_speed;
  timer_tima_acc_clocks_ &= (1 << related_speed) - 1;
  timer_tima_ += overflow;

  if (timer_tima_ < overflow) {
    // debug_log("timer interrupt!!");
    cpu->request_interrupt(CPUInterrupts::INT_TIMER);
    timer_tima_ = timer_tac_; // load the data of TAC;
  }
}

} // namespace GB
