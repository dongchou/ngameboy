#pragma once

#include "instruction.h"

namespace GB {
//============================================================
class CP_A_REG8_Inst : public Instruction {
public:
  CP_A_REG8_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t d1 = _cpu->reg(CPURegister::REG_A);
    reg8_t d2 = _cpu->reg(_param);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, d1 == d2);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, true);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, (d1 & 0xf) < (d2 & 0xf));
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, d1 < d2);
    return 4;
  }

  std::string name() override {
    return std::string("CP A,") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

class CP_A_HLP_Inst : public Instruction {
public:
  CP_A_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    reg8_t d1 = _cpu->reg(CPURegister::REG_A);
    reg8_t d2 = _cpu->memory(_cpu->reg(CPURegister::REG_HL));

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, d1 == d2);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, true);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, (d1 & 0xf) < (d2 & 0xf));
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, d1 < d2);
    return 8;
  }
  std::string name() override { return std::string("CP A,[HL]"); }
};

class CP_A_D8_Inst : public Instruction {
public:
  CP_A_D8_Inst(CPU *cpu, d8_t param) : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t d1 = _cpu->reg(CPURegister::REG_A);
    reg8_t d2 = _param;

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, d1 == d2);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, true);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, (d1 & 0xf) < (d2 & 0xf));
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, d1 < d2);
    return 8;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "CP A,$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << uint16_t(_param);
    return ss.str();
  }

public:
  d8_t _param;
};

//==================jump================
class JP_A16_Inst : public Instruction {
public:
  JP_A16_Inst(CPU *cpu, a16_t param) : Instruction(cpu), _param(param) {}

  int internal_exec() override {
    _cpu->reg(CPURegister::REG_PC, _param);
    return 16;
  }
  std::string name() override {
    std::ostringstream ss;
    ss << "JP $" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << _param;
    return ss.str();
  }

public:
  a16_t _param;
};

class JP_CC_A16_Inst : public Instruction {
public:
  JP_CC_A16_Inst(CPU *cpu, JumpCC cc, a16_t param)
      : Instruction(cpu), _cc(cc), _param(param) {}

  int internal_exec() override {
    bool e = false;
    switch (_cc) {
    case JumpCC::CC_NZ:
      e = !_cpu->test_flag(CPUFlagReg::FLAG_ZERO);
      break;
    case JumpCC::CC_NC:
      e = !_cpu->test_flag(CPUFlagReg::FLAG_CARRY);
      break;
    case JumpCC::CC_Z:
      e = _cpu->test_flag(CPUFlagReg::FLAG_ZERO);
      break;
    case JumpCC::CC_C:
      e = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
      break;
    default:
      assert(0);
    }
    if (e) {
      _cpu->reg(CPURegister::REG_PC, _param);
      return 16;
    }
    return 12;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "JP " << CPU::condition_name(_cc) << ",$" << std::uppercase
       << std::setw(4) << std::setfill('0') << std::hex << _param;
    return ss.str();
  }

public:
  JumpCC _cc;
  a16_t _param;
};

class JP_HLP_Inst : public Instruction {
public:
  JP_HLP_Inst(CPU *cpu) : Instruction(cpu) {}

  int internal_exec() override {
    _cpu->reg(CPURegister::REG_PC, _cpu->reg(CPURegister::REG_HL));
    return 4;
  }

  std::string name() override { return std::string("JP [HL]"); }
};

class JR_R8_Inst : public Instruction {
public:
  JR_R8_Inst(CPU *cpu, r8_t param) : Instruction(cpu), _param(param) {}

  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_PC);
    addr += _param;
    _cpu->reg(CPURegister::REG_PC, addr);
    return 12;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "JR " << int16_t(_param);
    return ss.str();
  }

public:
  r8_t _param;
};

class JR_CC_R8_Inst : public Instruction {
public:
  JR_CC_R8_Inst(CPU *cpu, JumpCC cc, r8_t param)
      : Instruction(cpu), _cc(cc), _param(param) {}

  int internal_exec() override {
    bool e = false;
    switch (_cc) {
    case JumpCC::CC_NZ:
      e = !_cpu->test_flag(CPUFlagReg::FLAG_ZERO);
      break;
    case JumpCC::CC_NC:
      e = !_cpu->test_flag(CPUFlagReg::FLAG_CARRY);
      break;
    case JumpCC::CC_Z:
      e = _cpu->test_flag(CPUFlagReg::FLAG_ZERO);
      break;
    case JumpCC::CC_C:
      e = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
      break;
    default:
      assert(0);
    }
    if (e) {
      a16_t addr = _cpu->reg(CPURegister::REG_PC);
      addr += _param;
      _cpu->reg(CPURegister::REG_PC, addr);
      return 12;
    }
    return 8;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "JR " << CPU::condition_name(_cc) << "," << int16_t(_param);
    return ss.str();
  }

public:
  JumpCC _cc;
  r8_t _param;
};

//=================call =============================
class CALL_A16_Inst : public Instruction {
public:
  CALL_A16_Inst(CPU *cpu, a16_t param) : Instruction(cpu), _param(param) {}

  int internal_exec() override {
    _cpu->push(_cpu->reg(CPURegister::REG_PC));
    _cpu->reg(CPURegister::REG_PC, _param);
    return 24;
  }
  std::string name() override {
    std::ostringstream ss;
    ss << "CALL $" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << _param;
    return ss.str();
  }

public:
  a16_t _param;
};

class CALL_CC_A16_Inst : public Instruction {
public:
  CALL_CC_A16_Inst(CPU *cpu, JumpCC cc, a16_t param)
      : Instruction(cpu), _cc(cc), _param(param) {}

  int internal_exec() override {
    bool e = false;
    switch (_cc) {
    case JumpCC::CC_NZ:
      e = !_cpu->test_flag(CPUFlagReg::FLAG_ZERO);
      break;
    case JumpCC::CC_NC:
      e = !_cpu->test_flag(CPUFlagReg::FLAG_CARRY);
      break;
    case JumpCC::CC_Z:
      e = _cpu->test_flag(CPUFlagReg::FLAG_ZERO);
      break;
    case JumpCC::CC_C:
      e = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
      break;
    default:
      assert(0);
    }
    if (e) {
      _cpu->push(_cpu->reg(CPURegister::REG_PC));
      _cpu->reg(CPURegister::REG_PC, _param);
      return 24;
    }
    return 12;
  }
  std::string name() override {
    std::ostringstream ss;
    ss << "CALL " << CPU::condition_name(_cc) << ",$" << std::uppercase
       << std::setw(4) << std::setfill('0') << std::hex << _param;
    return ss.str();
  }

public:
  JumpCC _cc;
  a16_t _param;
};

//==========================RET========================

class RET_Inst : public Instruction {
public:
  RET_Inst(CPU *cpu) : Instruction(cpu) {}

  int internal_exec() override {
    _cpu->reg(CPURegister::REG_PC, _cpu->pop());
    return 16;
  }
  std::string name() override { return std::string("RET"); }
};

class RET_CC_Inst : public Instruction {
public:
  RET_CC_Inst(CPU *cpu, JumpCC cc) : Instruction(cpu), _cc(cc) {}

  int internal_exec() override {
    bool e = false;
    switch (_cc) {
    case JumpCC::CC_NZ:
      e = !_cpu->test_flag(CPUFlagReg::FLAG_ZERO);
      break;
    case JumpCC::CC_NC:
      e = !_cpu->test_flag(CPUFlagReg::FLAG_CARRY);
      break;
    case JumpCC::CC_Z:
      e = _cpu->test_flag(CPUFlagReg::FLAG_ZERO);
      break;
    case JumpCC::CC_C:
      e = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
      break;
    default:
      assert(0);
    }
    if (e) {
      _cpu->reg(CPURegister::REG_PC, _cpu->pop());
      return 20;
    }
    return 8;
  }
  std::string name() override {
    return std::string("RET ") + CPU::condition_name(_cc);
  }

public:
  JumpCC _cc;
};

class RETI_Inst : public Instruction {
public:
  RETI_Inst(CPU *cpu) : Instruction(cpu) {}

  int internal_exec() override {
    _cpu->enable_all_interrupt();
    _cpu->reg(CPURegister::REG_PC, _cpu->pop());
    return 16;
  }
  std::string name() override { return std::string("RETI"); }
};
} // namespace GB