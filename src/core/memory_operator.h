#pragma once

#include "hardware.h"

namespace GB {

enum MappedIOPorts {
  REG_JOYPAD = 0xFF00,
  REG_IF = 0XFF0F,
  REG_DIV = 0XFF04,
  REG_TIMA = 0XFF05,
  REG_TMA = 0XFF06,
  REG_TAC = 0XFF07,

  REG_LCD_CTRL = 0xFF40,
  REG_LCD_STATUS = 0xFF41,
  REG_SCY = 0xFF42,
  REG_SCX = 0xFF43,
  REG_LY = 0xFF44,
  REG_LYC = 0xFF45,
  REG_BGP = 0xFF47,
  REG_BGP0 = 0xFF48,
  REG_BGP1 = 0xFF49,
  REG_WY = 0xFF4A,
  REG_WX = 0xFF4B,

  REG_DMA = 0XFF46,
  REG_TURN_OFF_ROM = 0xFF50,
  REG_IE = 0XFFFF,
};

class IPortOperator {
public:
  virtual ~IPortOperator() = default;
  virtual void set_reg(a16_t addr, byte data) = 0;
  virtual byte get_reg(a16_t addr) const = 0;
};

class MemoryOperator {
public:
  virtual ~MemoryOperator() = default;
  virtual void set(a16_t addr, byte data) = 0;
  virtual byte get(a16_t addr) const = 0;
};

} // namespace GB
