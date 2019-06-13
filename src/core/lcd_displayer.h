#pragma once

#include "hardware.h"
#include "joypad.h"
#include "pixelmap.h"
#include <GLFW/glfw3.h>
#include <atomic>
#include <list>
#include <mutex>
#include <thread>

namespace GB {


class LCDDisplayer{
public:
  virtual ~LCDDisplayer() = default;
  virtual bool prepare(Joypad *joypad) = 0;
  virtual void push_frame(const PixelMap &frame) = 0;
  virtual void run() = 0;
};

} // namespace GB
