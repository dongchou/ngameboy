#pragma once

#include "instruction.h"
namespace GB {
class LD_REG8_REG8_Inst : public Instruction {
public:
  LD_REG8_REG8_Inst(CPU *cpu, CPURegister dest, CPURegister src)
      : Instruction(cpu), _dest(dest), _src(src) {}
  int internal_exec() override {
    _cpu->reg(_dest, _cpu->reg(_src));
    return 4;
  }

  std::string name() override {
    return std::string("LD ") + CPU::reg_name(_dest) + "," +
           CPU::reg_name(_src);
  }

protected:
  CPURegister _dest;
  CPURegister _src;
};

class LD_REG8_D8_Inst : public Instruction {
public:
  LD_REG8_D8_Inst(CPU *cpu, CPURegister dest, d8_t param)
      : Instruction(cpu), _dest(dest), _param(param) {}
  int internal_exec() override {
    _cpu->reg(_dest, _param);
    return 8;
  }
  std::string name() override {
    std::ostringstream ss;
    ss << "LD " << CPU::reg_name(_dest) << ",$" << std::uppercase
       << std::setw(4) << std::setfill('0') << std::hex << uint16_t(_param);
    return ss.str();
  }

protected:
  CPURegister _dest;
  d8_t _param;
};

class LD_REG16_D16_Inst : public Instruction {
public:
  LD_REG16_D16_Inst(CPU *cpu, CPURegister dest, d16_t param)
      : Instruction(cpu), _dest(dest), _param(param) {}
  int internal_exec() override {
    _cpu->reg(_dest, _param);
    return 12;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "LD " << CPU::reg_name(_dest) << ",$" << std::uppercase
       << std::setw(4) << std::setfill('0') << std::hex << _param;
    return ss.str();
  }

protected:
  CPURegister _dest;
  d16_t _param;
};

class LD_HLP_REG8_Inst : public Instruction {
public:
  LD_HLP_REG8_Inst(CPU *cpu, CPURegister src) : Instruction(cpu), _src(src) {}
  int internal_exec() override {
    _cpu->memory(_cpu->reg(CPURegister::REG_HL), _cpu->reg(_src));
    return 8;
  }

  std::string name() override {
    return std::string("LD [HL],") + CPU::reg_name(_src);
  }

protected:
  CPURegister _src;
};

class LD_HLP_D8_Inst : public Instruction {
public:
  LD_HLP_D8_Inst(CPU *cpu, d8_t param) : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    _cpu->memory(_cpu->reg(CPURegister::REG_HL), _param);
    return 12;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "LD [HL],$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << uint16_t(_param);
    return ss.str();
  }

protected:
  d8_t _param;
};

class LD_REG8_HLP_Inst : public Instruction {
public:
  LD_REG8_HLP_Inst(CPU *cpu, CPURegister dest)
      : Instruction(cpu), _dest(dest) {}
  int internal_exec() override {
    _cpu->reg(_dest, _cpu->memory(_cpu->reg(CPURegister::REG_HL)));
    return 8;
  }

  std::string name() override {
    return std::string("LD ") + CPU::reg_name(_dest) + std::string(",[HL]");
  }

protected:
  CPURegister _dest;
};

class LD_REG16P_A_Inst : public Instruction {
public:
  LD_REG16P_A_Inst(CPU *cpu, CPURegister destP)
      : Instruction(cpu), _destP(destP) {}
  int internal_exec() override {
    _cpu->memory(_cpu->reg(_destP), _cpu->reg(CPURegister::REG_A));
    return 8;
  }

  std::string name() override {
    return std::string("LD [") + CPU::reg_name(_destP) + std::string("],A");
  }

protected:
  CPURegister _destP;
};

class LD_A16_A_Inst : public Instruction {
public:
  LD_A16_A_Inst(CPU *cpu, a16_t addr) : Instruction(cpu), _addr(addr) {}
  int internal_exec() override {
    _cpu->memory(_addr, _cpu->reg(CPURegister::REG_A));
    return 16;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "LD [$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << _addr << "],A";
    return ss.str();
  }

protected:
  a16_t _addr;
};

// class LD_D8_A_Inst : public Instruction {
// public:
//   LD_D8_A_Inst(CPU *cpu, d8_t offset) : Instruction(cpu), _offset(offset) {}
//   int internal_exec() override {
//     _cpu->memory(0xFF00 + _offset, _cpu->reg(CPURegister::REG_A));
//   }
//   std::string name() override {
//     std::ostringstream ss;
//     ss << "LD $FF00+$" << std::uppercase << std::setw(4) << std::setfill('0')
//        << std::hex << _offset << ",A";
//     return ss.str();
//   }

// protected:
//   d8_t _offset;
// };

class LD_CP_A_Inst : public Instruction {
public:
  LD_CP_A_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    _cpu->memory(0xFF00 + _cpu->reg(CPURegister::REG_C),
                 _cpu->reg(CPURegister::REG_A));
    return 8;
  }
  std::string name() override { return std::string("LD [$FF00+C],A"); }
};

class LD_A_REG16P_Inst : public Instruction {
public:
  LD_A_REG16P_Inst(CPU *cpu, CPURegister srcP)
      : Instruction(cpu), _srcP(srcP) {}
  int internal_exec() override {
    a16_t addr = _cpu->reg(_srcP);
    _cpu->reg(CPURegister::REG_A, _cpu->memory(addr));
    return 8;
  }

  std::string name() override {
    return std::string("LD A,[") + CPU::reg_name(_srcP) + std::string("]");
  }

protected:
  CPURegister _srcP;
};

class LD_A_A16_Inst : public Instruction {
public:
  LD_A_A16_Inst(CPU *cpu, a16_t srcP) : Instruction(cpu), _srcP(srcP) {}
  int internal_exec() override {
    _cpu->reg(CPURegister::REG_A, _cpu->memory(_srcP));
    return 16;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "LD A,[$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << _srcP << "]";
    return ss.str();
  }

protected:
  a16_t _srcP;
};

class LDH_A_A8_Inst : public Instruction {
public:
  LDH_A_A8_Inst(CPU *cpu, a8_t srcP) : Instruction(cpu), _srcP(srcP) {}
  int internal_exec() override {
    _cpu->reg(CPURegister::REG_A, _cpu->memory(0xFF00 + _srcP));
    return 12;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "LD A,[$FF00+$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << uint16_t(_srcP) << "]";
    return ss.str();
  }

protected:
  a8_t _srcP;
};

class LDH_A8_A_Inst : public Instruction {
public:
  LDH_A8_A_Inst(CPU *cpu, a8_t srcP) : Instruction(cpu), _srcP(srcP) {}
  int internal_exec() override {
    _cpu->memory(0xFF00 + _srcP,
                 static_cast<byte>(_cpu->reg(CPURegister::REG_A)));
    return 12;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "LD [$FF00+$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << uint16_t(_srcP) << "],A";
    return ss.str();
  }

protected:
  a8_t _srcP;
};

class LD_A_CP_Inst : public Instruction {
public:
  LD_A_CP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    _cpu->reg(CPURegister::REG_A,
              _cpu->memory(0xFF00 + _cpu->reg(CPURegister::REG_C)));
    return 8;
  }

  std::string name() override { return std::string("LD A,[$FF00+C]"); }
};

//[HL+] post-increment
class LDI_HLP_A_Inst : public Instruction {
public:
  LDI_HLP_A_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    a16_t addr = _cpu->incr(CPURegister::REG_HL);
    _cpu->memory(addr, _cpu->reg(CPURegister::REG_A));
    return 8;
  }
  std::string name() override { return std::string("LDI [HL],A"); }
};

//[HL+] post-decrement
class LDD_HLP_A_Inst : public Instruction {
public:
  LDD_HLP_A_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    a16_t addr = _cpu->decr(CPURegister::REG_HL);
    _cpu->memory(addr, _cpu->reg(CPURegister::REG_A));
    return 8;
  }
  std::string name() override { return std::string("LDD [HL],A"); }
};
//[HL+] post-increment
class LDI_A_HLP_Inst : public Instruction {
public:
  LDI_A_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    a16_t addr = _cpu->incr(CPURegister::REG_HL);
    _cpu->reg(CPURegister::REG_A, _cpu->memory(addr));
    return 8;
  }
  std::string name() override { return std::string("LDI A,[HL]"); }
};

//[HL+] post-decrement
class LDD_A_HLP_Inst : public Instruction {
public:
  LDD_A_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    a16_t addr = _cpu->decr(CPURegister::REG_HL);
    _cpu->reg(CPURegister::REG_A, _cpu->memory(addr));
    return 8;
  }
  std::string name() override { return std::string("LDD A,[HL]"); }
};

class LD_SP_D16_Inst : public Instruction {
public:
  LD_SP_D16_Inst(CPU *cpu, d16_t param) : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    _cpu->reg(CPURegister::REG_SP, _param);
    return 12;
  }
  std::string name() override {
    std::ostringstream ss;
    ss << "LD SP,$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << _param;
    return ss.str();
  }

public:
  d16_t _param;
};

class LD_D16P_SP_Inst : public Instruction {
public:
  LD_D16P_SP_Inst(CPU *cpu, d16_t addr) : Instruction(cpu), _addr(addr) {}
  int internal_exec() override {
    reg16_t data = _cpu->reg(CPURegister::REG_SP);
    _cpu->memory(_addr, byte(data & 0xff));
    _cpu->memory(_addr + 1, byte(data >> 8));
    return 20;
  }
  std::string name() override {
    std::ostringstream ss;
    ss << "LD [$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << _addr << "],SP";
    return ss.str();
  }

public:
  d16_t _addr;
};

class LDHL_SP_R8_Inst : public Instruction {
public:
  LDHL_SP_R8_Inst(CPU *cpu, r8_t param) : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_SP);
    a16_t result = addr + _param;
    _cpu->reg(CPURegister::REG_HL, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, false);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY,
                   (addr ^ _param ^ result) & 0x10);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, (addr ^ _param ^ result) & 0x100);
    return 12;
  }
  std::string name() override {
    std::ostringstream ss;
    ss << "LDHL SP," << int16_t(_param);
    return ss.str();
  }

public:
  r8_t _param;
};

class LD_SP_HL_Inst : public Instruction {
public:
  LD_SP_HL_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    _cpu->reg(CPURegister::REG_SP, _cpu->reg(CPURegister::REG_HL));
    return 8;
  }
  std::string name() override { return std::string("LD SP,HL"); }
};
} // namespace GB