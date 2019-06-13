#pragma once

#include "common.h"
#include "hardware.h"
#include "memory.h"
#include "timer.h"
#include <memory>
#include <string>

namespace GB {
/*
reg8_t:   r8	  (8bit register)
reg16_t:  r16	  (16bit register)
d8_t:     n8		(8bit immediate data)
d16_t:    n16		(16bit immediate data)
r8_t:     e8		(8bit signed data, which are added to program counter)
a16_t	(address 16)
a8_t	(8bit address,often usage: 0xFF00+a8_t)
cc		(condition codes)
vec
*/
enum CPURegister {
  REG_MIN = 0,
  REG_A = REG_MIN,
  REG_F, // used to store the results of various math operations.
  REG_B,
  REG_C,
  REG_D,
  REG_E,
  REG_H,
  REG_L,
  REG_SP, // stack pointer
  REG_PC, // program counter
  REG_MAX,

  REG_AF,
  REG_BC,
  REG_DE,
  REG_HL,
};

enum CPUFlagReg {
  FLAG_ZERO = 0x7,       // Z
  FLAG_SUB = 0x6,        // N
  FLAG_HALF_CARRY = 0x5, // H
  FLAG_CARRY = 0x4,      // C
};

enum JumpCC {
  CC_NZ,
  CC_NC,
  CC_Z,
  CC_C,
};

constexpr int NORMAL_CLOCK_FREQUENCY = 4194304;
enum CPUInterrupts {
  INT_V_BLANK = 0,  /*Bit 0: V-Blank   (INT 40h)*/
  INT_LCD_STAT = 1, /*Bit 1: LCD STAT  (INT 48h) */
  INT_TIMER = 2,    /*Bit 2: Timer (INT 50h)*/
  INT_SERIAL = 3,   /*Bit 3: Serial (INT 58h)*/
  INT_JOYPAD = 4,   /*Bit 4: Joypad (INT 60h)*/
  INT_NONE = 5,
};

constexpr int MAX_INTERRUPT_VALUE = (1 << INT_V_BLANK) | (1 << INT_LCD_STAT) |
                                    (1 << INT_TIMER) | (1 << INT_SERIAL) |
                                    (1 << INT_JOYPAD);

struct CPURunStatus {
  bool disable_rom;
  bool debug_mode;
};

class CPU final : public non_copyable {
public:
  CPU(Memory *memory, bool debug_mode = true);
  ~CPU();

  static std::string reg_name(CPURegister reg);
  static std::string condition_name(JumpCC cc);
  reg16_t reg(CPURegister r) const; // single register
  void reg(CPURegister r, reg16_t v);

  reg16_t incr(CPURegister r) {
    auto v = reg(r);
    reg(r, v + 1);
    return v;
  }

  reg16_t decr(CPURegister r) {
    auto v = reg(r);
    reg(r, v - 1);
    return v;
  }

  bool test_flag(CPUFlagReg f) const { return _registers[REG_F] & (1U << f); }

  void set_flag(CPUFlagReg f, bool b) {
    if (b) {
      _registers[REG_F] |= (1U << f);
    } else {
      _registers[REG_F] &= (~(1U << f));
    }
  }

  byte memory(size_t index) const;

  void memory(size_t index, byte v);

  reg16_t pop();
  void push(reg16_t v);

  void disable_all_interrupt() { _ime = false; }
  void enable_all_interrupt() { _ime = true; }

  std::string debug_info() const;

  void set_ie(uint8_t data);
  void set_if(uint8_t data);

  void request_interrupt(CPUInterrupts i) { SET_BIT(_interrupt_flags, i); }

  uint8_t get_ie() const { return _interrupt_enable; }
  uint8_t get_if() const { return _interrupt_flags; }

  bool check_enable(CPUInterrupts i) const {
    if (!_ime) {
      return false;
    }
    return (_interrupt_enable & (1 << i)) != 0;
  }

  void handle_interrupts();

  int update();
  void halt() { halted_ = true; }

  uint32_t frequency() const {
    return double_speed_mode_ ? 2 * clock_frequency_ : clock_frequency_;
  }

private:
  bool debug_mode_;
  reg16_t _registers[CPURegister::REG_MAX];
  Memory *_memory;

  bool double_speed_mode_;
  uint32_t clock_frequency_;

  bool halted_;
  // Interrupt Implementations
  bool _ime; //  master enable flag
  uint8_t _interrupt_enable;
  uint8_t _interrupt_flags;

  // Timer Interrupt Implementation
  std::shared_ptr<Timer> timer_;

  // Serial Interrupt Implementation

  // JoyPad Interrupt Implementation
};
} // namespace GB