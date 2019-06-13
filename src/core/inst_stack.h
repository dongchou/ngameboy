#pragma once

#include "instruction.h"

namespace GB {
//=======================STACK OPERATION===================
// class POP_AF_Inst : public Instruction {
// public:
//   POP_AF_Inst(CPU *cpu) : Instruction(cpu) {}
//   int internal_exec() override {
//     reg16_t v = _cpu->pop();
//     _cpu->reg(CPURegister::REG_AF, v);

//     _cpu->set_flag(CPUFlagReg::FLAG_ZERO, ((v & 0xFF) & (1 << 7)) == 1);
//     _cpu->set_flag(CPUFlagReg::FLAG_SUB, ((v & 0xFF) & (1 << 6)) == 1);
//     _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, ((v & 0xFF) & (1 << 5)) ==
//     1); _cpu->set_flag(CPUFlagReg::FLAG_CARRY, ((v & 0xFF) & (1 << 4)) == 1);
//     return 12;
//   }
//   std::string name() override { return std::string("POP AF"); }
// };

class POP_REG16_Inst : public Instruction {
public:
  POP_REG16_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    _cpu->reg(_param, _cpu->pop());
    return 12;
  }

  std::string name() override {
    return std::string("POP ") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

// class PUSH_AF_Inst : public Instruction {
// public:
//   PUSH_AF_Inst(CPU *cpu) : Instruction(cpu) {}
//   int internal_exec() override {
//     reg16_t v = _cpu->reg(CPURegister::REG_AF);
//     _cpu->push(v);

//     _cpu->set_flag(CPUFlagReg::FLAG_ZERO, ((v & 0xFF) & (1 << 7)) == 1);
//     _cpu->set_flag(CPUFlagReg::FLAG_SUB, ((v & 0xFF) & (1 << 6)) == 1);
//     _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, ((v & 0xFF) & (1 << 5)) ==
//     1); _cpu->set_flag(CPUFlagReg::FLAG_CARRY, ((v & 0xFF) & (1 << 4)) == 1);
//     return 16;
//   }
//   std::string name() override { return std::string("PUSH AF"); }
// };

class PUSH_REG16_Inst : public Instruction {
public:
  PUSH_REG16_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    _cpu->push(_cpu->reg(_param));
    return 16;
  }

  std::string name() override {
    return std::string("PUSH ") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};
} // namespace GB