#pragma once

#include "cpu.h"
#include <chrono>
#include <thread>

#define HALF_CARRY_ADD_TEST(a, b) (((((a)&0xf) + ((b)&0xf)) & 0x10) != 0)
#define HALF_CARRY_SUB_TEST(a, b) ((((a)&0xf) < ((b)&0xf)))

namespace GB {
class Instruction {
public:
  Instruction(CPU *cpu) : _cpu(cpu) {}
  virtual ~Instruction() {}
  int execute() {
    return internal_exec();
    // auto cycles = internal_exec();
    // std::this_thread::sleep_for(
    //     std::chrono::microseconds(static_cast<int>(cycles * 0.25f)));
  }

  virtual std::string name() = 0;

protected:
  virtual int internal_exec() = 0;

protected:
  CPU *_cpu;
};

} // namespace GB