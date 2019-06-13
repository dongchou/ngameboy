

#include "lcd_controller.h"
#include <cstring>

namespace GB {
LCDController::LCDController(Memory *memory) : _memory(memory) {}
LCDController::~LCDController() {}

} // namespace GB