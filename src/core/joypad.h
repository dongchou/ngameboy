#pragma once

#include "common.h"
#include "hardware.h"
#include "memory_operator.h"
#include <chrono>
#include <string>
#include <thread>

namespace GB {
/*
  Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)
  Bit 1 - P11 Input Left  or Button B (0=Pressed) (Read Only)
  Bit 2 - P12 Input Up    or Select   (0=Pressed) (Read Only)
  Bit 3 - P13 Input Down  or Start    (0=Pressed) (Read Only)
  Bit 4 - P14 Select Direction Keys   (0=Select)
  Bit 5 - P15 Select Button Keys      (0=Select)
  Bit 6 - Not used
  Bit 7 - Not used
 */
enum JoypadKeyBit {
  KEY_P10 = 0,
  KEY_P11 = 1,
  KEY_P12 = 2,
  KEY_P13 = 3,
  KEY_P14 = 4,
  KEY_P15 = 5,
};

enum JoypadKey {
  KEY_RIGHT = 0x00,
  KEY_LEFT = 0x01,
  KEY_UP = 0x02,
  KEY_DOWN = 0x3,

  KEY_BUTTON_A = 0x10,
  KEY_BUTTON_B = 0x11,
  KEY_SELECT = 0x12,
  KEY_START = 0x13,

  KEY_NONE = 0xff,
};

class Joypad final : public IPortOperator {
  enum KeyOption {
    Direction = 0,
    Button = 1,
    None,
  };

public:
  Joypad() : opt_(KeyOption::None) {
    key_flags_[0] = 0xf;
    key_flags_[1] = 0xf;
  }
  ~Joypad() {}

  void update_key(JoypadKey key, bool pressed) {
    if (key == JoypadKey::KEY_NONE) {
      return;
    }

    int keyopt = (key >> 4) & 1;
    uint8_t flags = key_flags_[keyopt];
    if (pressed) {
      CLEAR_BIT(flags, key & 0xf);
    } else {
      SET_BIT(flags, key & 0xf);
    }
    key_flags_[keyopt] = flags;
  }

  void set_reg(a16_t addr, byte data) override {
    if (!IS_BIT_SET(data, JoypadKeyBit::KEY_P14)) {
      opt_ = KeyOption::Direction;
    } else if (!IS_BIT_SET(data, JoypadKeyBit::KEY_P15)) {
      opt_ = KeyOption::Button;
    } else {
      opt_ = KeyOption::None;
    }
  }

  byte get_reg(a16_t addr) const override {
    byte data = 0;
    if (opt_ == KeyOption::Direction) {
      SET_BIT(data, JoypadKeyBit::KEY_P14);
    } else if (opt_ == KeyOption::Button) {
      SET_BIT(data, JoypadKeyBit::KEY_P15);
    } else {
      return 0x3f;
    }
    return data | key_flags_[opt_];
  }

private:
  KeyOption opt_;
  volatile byte key_flags_[2];
}; // namespace GB

} // namespace GB