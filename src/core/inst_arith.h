#pragma once

#include "instruction.h"
#include <iomanip>
#include <sstream>
namespace GB {
//=================================INC=========================
class INC_REG8_Inst : public Instruction {
public:
  INC_REG8_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(_param);
    reg8_t result = dest + 1;
    _cpu->reg(_param, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, HALF_CARRY_ADD_TEST(dest, 1));
    return 4;
  }

  std::string name() override {
    return std::string("INC ") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

class INC_HLP_Inst : public Instruction {
public:
  INC_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t dest = _cpu->memory(addr);
    reg8_t result = dest + 1;
    _cpu->memory(addr, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, HALF_CARRY_ADD_TEST(dest, 1));
    return 12;
  }

  std::string name() override { return "INC [HL]"; }
};

class INC_REG16_Inst : public Instruction {
public:
  INC_REG16_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    _cpu->incr(_param);
    return 8;
  }

  std::string name() override {
    return std::string("INC ") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

class INC_SP_Inst : public Instruction {
public:
  INC_SP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    _cpu->incr(CPURegister::REG_SP);
    return 8;
  }

  std::string name() override { return "INC SP"; }
};

//===============================DEC==================================
class DEC_REG8_Inst : public Instruction {
public:
  DEC_REG8_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(_param);
    reg8_t result = dest - 1;
    _cpu->reg(_param, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, true);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, HALF_CARRY_SUB_TEST(dest, 1));
    return 4;
  }

  std::string name() override {
    return std::string("DEC ") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

class DEC_HLP_Inst : public Instruction {
public:
  DEC_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t dest = _cpu->memory(addr);
    reg8_t result = dest - 1;
    _cpu->memory(addr, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, true);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, HALF_CARRY_SUB_TEST(dest, 1));
    return 12;
  }
  std::string name() override { return "DEC [HL]"; }
};

class DEC_REG16_Inst : public Instruction {
public:
  DEC_REG16_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    _cpu->decr(_param);
    return 8;
  }

  std::string name() override {
    return std::string("DEC ") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

class DEC_SP_Inst : public Instruction {
public:
  DEC_SP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    _cpu->decr(CPURegister::REG_SP);
    return 8;
  }
  std::string name() override { return "DEC SP"; }
};
////////////////////////////////////Add instruction
class ADD_A_REG8_Inst : public Instruction {
public:
  ADD_A_REG8_Inst(CPU *cpu, CPURegister src) : Instruction(cpu), _src(src) {}
  int internal_exec() override {
    reg16_t dest = _cpu->reg(CPURegister::REG_A);
    reg16_t src = _cpu->reg(_src);
    reg16_t real_result = dest + src;
    reg8_t result = static_cast<reg8_t>(real_result);
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, HALF_CARRY_ADD_TEST(dest, src));
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, real_result & 0x100);
    return 4;
  }

  std::string name() override {
    return std::string("ADD A,") + CPU::reg_name(_src);
  }

protected:
  CPURegister _src;
};

class ADD_A_HLP_Inst : public Instruction {
public:
  ADD_A_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    reg8_t v = _cpu->reg(CPURegister::REG_A);
    reg8_t s = _cpu->memory(_cpu->reg(CPURegister::REG_HL));
    reg16_t tmp = v + s;
    reg8_t result = tmp;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, HALF_CARRY_ADD_TEST(v, s));
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, tmp & 0x100);
    return 8;
  }

  std::string name() override { return std::string("ADD A,[HL]"); }
};

class ADD_A_D8_Inst : public Instruction {
public:
  ADD_A_D8_Inst(CPU *cpu, d8_t src) : Instruction(cpu), _src(src) {}
  int internal_exec() override {
    reg8_t v = _cpu->reg(CPURegister::REG_A);
    reg16_t tmp = v + _src;
    reg8_t result = tmp;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, HALF_CARRY_ADD_TEST(v, _src));
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, tmp & 0x100);
    return 8;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "ADD A,$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << uint16_t(_src);
    return ss.str();
  }

protected:
  d8_t _src;
};

class ADD_HL_REG16_Inst : public Instruction {
public:
  ADD_HL_REG16_Inst(CPU *cpu, CPURegister src) : Instruction(cpu), _src(src) {}
  int internal_exec() override {
    reg16_t dest = _cpu->reg(CPURegister::REG_HL);
    reg16_t src = _cpu->reg(_src);
    uint32_t result = dest + src;
    _cpu->reg(CPURegister::REG_HL, (reg16_t)result);

    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY,
                   ((dest & 0xfff) + (src & 0xfff)) & 0x1000);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, result & 0x10000);
    return 8;
  }

  std::string name() override {
    return std::string("ADD HL,") + CPU::reg_name(_src);
  }

protected:
  CPURegister _src;
};

class ADD_HL_SP_Inst : public Instruction {
public:
  ADD_HL_SP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    auto v = _cpu->reg(CPURegister::REG_HL);
    auto s = _cpu->reg(CPURegister::REG_SP);
    uint32_t n = v + s;
    _cpu->reg(CPURegister::REG_HL, reg16_t(n));

    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY,
                   ((v & 0xfff) + (s & 0xfff)) & 0x1000);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, n & 0x10000);
    return 8;
  }
  std::string name() override { return std::string("ADD HL,SP"); }
};

class ADD_SP_R8_Inst : public Instruction {
public:
  ADD_SP_R8_Inst(CPU *cpu, r8_t param) : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg16_t v = _cpu->reg(CPURegister::REG_SP);
    reg16_t n = v + _param;
    _cpu->reg(CPURegister::REG_SP, n);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, false);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, (v ^ _param ^ n) & 0x10);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, (v ^ _param ^ n) & 0x100);
    return 16;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "ADD SP,$" << int16_t(_param);
    return ss.str();
  }

protected:
  r8_t _param;
};

///======================================== SUB instruction
class SUB_A_REG8_Inst : public Instruction {
public:
  SUB_A_REG8_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _cpu->reg(_param);
    reg8_t result = dest - src;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, true);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, HALF_CARRY_SUB_TEST(dest, src));
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, dest < src);
    return 4;
  }

  std::string name() override {
    return std::string("SUB A,") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

class SUB_A_HLP_Inst : public Instruction {
public:
  SUB_A_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _cpu->memory(_cpu->reg(CPURegister::REG_HL));
    reg8_t result = dest - src;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, true);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, HALF_CARRY_SUB_TEST(dest, src));
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, dest < src);
    return 8;
  }
  std::string name() override { return std::string("SUB A,[HL]"); }
};

class SUB_A_D8_Inst : public Instruction {
public:
  SUB_A_D8_Inst(CPU *cpu, d8_t param) : Instruction(cpu), _param(param) {}

  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _param;
    reg8_t result = dest - src;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, true);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, HALF_CARRY_SUB_TEST(dest, src));
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, dest < src);
    return 8;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "SUB A,$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << uint16_t(_param);
    return ss.str();
  }

protected:
  d8_t _param;
};
//===================================ADC===================================
class ADC_A_REG8_Inst : public Instruction {
public:
  ADC_A_REG8_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _cpu->reg(_param);
    d8_t carry = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
    d16_t tmp = dest + src + carry;
    reg8_t result = static_cast<reg8_t>(tmp);
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY,
                   ((dest & 0xf) + (src & 0xf) + carry) & 0x10);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, tmp > 0xff);
    return 4;
  }

  std::string name() override {
    return std::string("ADC A,") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

class ADC_A_HLP_Inst : public Instruction {
public:
  ADC_A_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _cpu->memory(_cpu->reg(CPURegister::REG_HL));
    d8_t carry = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
    d16_t tmp = dest + src + carry;
    reg8_t result = static_cast<reg8_t>(tmp);
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY,
                   ((dest & 0xf) + (src & 0xf) + carry) & 0x10);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, tmp > 0xff);
    return 8;
  }
  std::string name() override { return std::string("ADC A,[HL]"); }
};

class ADC_A_D8_Inst : public Instruction {
public:
  ADC_A_D8_Inst(CPU *cpu, d8_t param) : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _param;
    d8_t carry = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
    d16_t tmp = dest + src + carry;
    reg8_t result = static_cast<reg8_t>(tmp);
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY,
                   ((dest & 0xf) + (src & 0xf) + carry) & 0x10);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, tmp > 0xff);
    return 8;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "ADC A,$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << uint16_t(_param);
    return ss.str();
  }

protected:
  d8_t _param;
};

//===================================SBC===================================
class SBC_A_REG8_Inst : public Instruction {
public:
  SBC_A_REG8_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _cpu->reg(_param);
    d16_t carry = _cpu->test_flag(CPUFlagReg::FLAG_CARRY) ? 1 : 0;
    reg8_t result = dest - src - carry;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, true);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY,
                   (dest & 0xf) < (src & 0xf) + carry);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, dest < src + carry);
    return 4;
  }

  std::string name() override {
    return std::string("SBC A,") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

class SBC_A_HLP_Inst : public Instruction {
public:
  SBC_A_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _cpu->memory(_cpu->reg(CPURegister::REG_HL));
    d16_t carry = _cpu->test_flag(CPUFlagReg::FLAG_CARRY) ? 1 : 0;
    reg8_t result = dest - src - carry;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, true);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY,
                   (dest & 0xf) < (src & 0xf) + carry);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, dest < src + carry);
    return 8;
  }
  std::string name() override { return std::string("SBC A,[HL]"); }
};

class SBC_A_D8_Inst : public Instruction {
public:
  SBC_A_D8_Inst(CPU *cpu, d8_t param) : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _param;
    d16_t carry = _cpu->test_flag(CPUFlagReg::FLAG_CARRY) ? 1 : 0;
    reg8_t result = dest - src - carry;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, true);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY,
                   (dest & 0xf) < (src & 0xf) + carry);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, dest < src + carry);
    return 8;
  }
  std::string name() override {
    std::ostringstream ss;
    ss << "SBC A,$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << uint16_t(_param);
    return ss.str();
  }

protected:
  d8_t _param;
};

class SCF_Inst : public Instruction {
public:
  SCF_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, true);
    return 4;
  }
  std::string name() override { return std::string("SCF"); }
};
} // namespace GB