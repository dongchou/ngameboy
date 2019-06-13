#pragma once

#include "cpu.h"
#include "memory.h"
#include "memory_operator.h"
#include "pixelmap.h"
#include <memory>
#include <vector>

namespace GB {

class LCDController final {
public:
  LCDController(Memory *memory);
  ~LCDController();

protected:
  Memory *_memory;
};

} // namespace GB