#pragma once

#include "instruction.h"

namespace GB {
//============================AND=========================
class AND_A_REG8_Inst : public Instruction {
public:
  AND_A_REG8_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _cpu->reg(_param);
    reg8_t result = dest & src;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, true);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, false);
    return 4;
  }

  std::string name() override {
    return std::string("AND A,") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

class AND_A_HLP_Inst : public Instruction {
public:
  AND_A_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _cpu->memory(_cpu->reg(CPURegister::REG_HL));
    reg8_t result = dest & src;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, true);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, false);
    return 8;
  }
  std::string name() override { return std::string("AND A,[HL]"); }
};

class AND_A_D8_Inst : public Instruction {
public:
  AND_A_D8_Inst(CPU *cpu, d8_t param) : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _param;
    reg8_t result = dest & src;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, true);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, false);
    return 8;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "AND A,$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << uint16_t(_param);
    return ss.str();
  }

protected:
  d8_t _param;
};

//======================XOR=================================-
class XOR_A_REG8_Inst : public Instruction {
public:
  XOR_A_REG8_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _cpu->reg(_param);
    reg8_t result = dest ^ src;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, false);
    return 4;
  }

  std::string name() override {
    return std::string("XOR A,") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

class XOR_A_HLP_Inst : public Instruction {
public:
  XOR_A_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _cpu->memory(_cpu->reg(CPURegister::REG_HL));
    reg8_t result = dest ^ src;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, false);
    return 8;
  }

  std::string name() override { return std::string("XOR A,[HL]"); }
};

class XOR_A_D8_Inst : public Instruction {
public:
  XOR_A_D8_Inst(CPU *cpu, d8_t param) : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _param;
    reg8_t result = dest ^ src;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, false);
    return 8;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "XOR A,$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << uint16_t(_param);
    return ss.str();
  }

protected:
  d8_t _param;
};

//======================OR
class OR_A_REG8_Inst : public Instruction {
public:
  OR_A_REG8_Inst(CPU *cpu, CPURegister param)
      : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _cpu->reg(_param);
    reg8_t result = dest | src;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, false);
    return 4;
  }

  std::string name() override {
    return std::string("OR A,") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

class OR_A_HLP_Inst : public Instruction {
public:
  OR_A_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _cpu->memory(_cpu->reg(CPURegister::REG_HL));
    reg8_t result = dest | src;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, false);
    return 8;
  }

  std::string name() override { return std::string("OR A,[HL]"); }
};

class OR_A_D8_Inst : public Instruction {
public:
  OR_A_D8_Inst(CPU *cpu, d8_t param) : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t src = _param;
    reg8_t result = dest | src;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, false);
    return 8;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "OR A,$" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << uint16_t(_param);
    return ss.str();
  }

protected:
  d8_t _param;
};

//=============================RL ================================
class RL_REG8_Inst : public Instruction {
public:
  RL_REG8_Inst(CPU *cpu, CPURegister param) : Instruction(cpu), _param(param) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(_param);
    reg8_t prevc = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
    reg8_t result = dest << 1 | prevc;
    _cpu->reg(_param, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, (dest >> 7));
    return 8;
  }

  std::string name() override {
    return std::string("RL ") + CPU::reg_name(_param);
  }

protected:
  CPURegister _param;
};

class RL_HLP_Inst : public Instruction {
public:
  RL_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t dest = _cpu->memory(addr);
    reg8_t prevc = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
    reg8_t result = dest << 1 | prevc;
    _cpu->memory(addr, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, (dest >> 7));
    return 16;
  }
  std::string name() override { return std::string("RL [HL]"); }
};

class RLA_Inst : public Instruction {
public:
  RLA_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t prevc = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
    reg8_t result = dest << 1 | prevc;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, false);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, (dest >> 7));
    return 4;
  }

  std::string name() override { return std::string("RLA"); }
};

class RLC_REG8_Inst : public Instruction {
public:
  RLC_REG8_Inst(CPU *cpu, CPURegister dest) : Instruction(cpu), _dest(dest) {}
  int internal_exec() override {
    reg8_t value = _cpu->reg(_dest);
    reg8_t result = value << 1 | value >> 7;
    _cpu->reg(_dest, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, (value >> 7));
    return 8;
  }

  std::string name() override {
    return std::string("RLC ") + CPU::reg_name(_dest);
  }

protected:
  CPURegister _dest;
};

class RLC_HLP_Inst : public Instruction {
public:
  RLC_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t value = _cpu->memory(addr);
    reg8_t result = value << 1 | value >> 7;
    _cpu->memory(addr, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, (value >> 7));
    return 16;
  }

  std::string name() override { return std::string("RLC [HL]"); }
};

class RLCA_Inst : public Instruction {
public:
  RLCA_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    reg8_t value = _cpu->reg(CPURegister::REG_A);
    reg8_t result = value << 1 | value >> 7;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, false);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, (value >> 7));
    return 4;
  }
  std::string name() override { return std::string("RLCA"); }
};

class RR_REG8_Inst : public Instruction {
public:
  RR_REG8_Inst(CPU *cpu, CPURegister dest) : Instruction(cpu), _dest(dest) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(_dest);
    reg8_t prevc = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
    reg8_t result = prevc << 7 | dest >> 1;
    _cpu->reg(_dest, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, (dest & 0x1));
    return 8;
  }

  std::string name() override {
    return std::string("RR ") + CPU::reg_name(_dest);
  }

protected:
  CPURegister _dest;
};

class RR_HLP_Inst : public Instruction {
public:
  RR_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t dest = _cpu->memory(addr);
    reg8_t prevc = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
    reg8_t result = prevc << 7 | dest >> 1;
    _cpu->memory(addr, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, (dest & 0x1));
    return 16;
  }
  std::string name() override { return std::string("RR [HL]"); }
};

class RRA_Inst : public Instruction {
public:
  RRA_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    reg8_t dest = _cpu->reg(CPURegister::REG_A);
    reg8_t prevc = _cpu->test_flag(CPUFlagReg::FLAG_CARRY);
    reg8_t result = prevc << 7 | dest >> 1;
    _cpu->reg(CPURegister::REG_A, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, false);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, (dest & 0x1));
    return 4;
  }
  std::string name() override { return std::string("RRA"); }
};

class RRC_REG8_Inst : public Instruction {
public:
  RRC_REG8_Inst(CPU *cpu, CPURegister dest) : Instruction(cpu), _dest(dest) {}
  int internal_exec() override {
    reg8_t value = _cpu->reg(_dest);
    uint8_t carry = value & 0x1;
    reg8_t result = (value >> 1) | (carry << 7);

    _cpu->reg(_dest, result);
    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, carry);
    return 8;
  }

  std::string name() override {
    return std::string("RRC ") + CPU::reg_name(_dest);
  }

protected:
  CPURegister _dest;
};

class RRC_HLP_Inst : public Instruction {
public:
  RRC_HLP_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t value = _cpu->memory(addr);
    uint8_t carry = value & 0x1;
    reg8_t result = (value >> 1) | (carry << 7);
    _cpu->memory(addr, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, carry);
    return 16;
  }

  std::string name() override { return std::string("RRC [HL]"); }
};

class RRCA_Inst : public Instruction {
public:
  RRCA_Inst(CPU *cpu) : Instruction(cpu) {}
  int internal_exec() override {
    reg8_t value = _cpu->reg(CPURegister::REG_A);
    uint8_t carry = value & 0x1;
    reg8_t result = (value >> 1) | (carry << 7);
    _cpu->reg(CPURegister::REG_A, result);
    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, false);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, carry);
    return 4;
  }
  std::string name() override { return std::string("RRCA"); }
};

//===========================SWAP========================
class SWAP_REG8_Inst : public Instruction {
public:
  SWAP_REG8_Inst(CPU *cpu, CPURegister dest) : Instruction(cpu), _dest(dest) {}

  int internal_exec() override {
    reg8_t value = _cpu->reg(_dest);
    reg8_t result = value << 4 | value >> 4;
    _cpu->reg(_dest, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, false);
    return 8;
  }

  std::string name() override {
    return std::string("SWAP ") + CPU::reg_name(_dest);
  }

protected:
  CPURegister _dest;
};

class SWAP_HLP_Inst : public Instruction {
public:
  SWAP_HLP_Inst(CPU *cpu) : Instruction(cpu) {}

  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t value = _cpu->memory(addr);
    reg8_t result = value << 4 | value >> 4;
    _cpu->memory(addr, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, false);
    return 16;
  }
  std::string name() override { return std::string("SWAP [HL]"); }
};

//===========================SLA========================
class SLA_REG8_Inst : public Instruction {
public:
  SLA_REG8_Inst(CPU *cpu, CPURegister dest) : Instruction(cpu), _dest(dest) {}

  int internal_exec() override {
    reg8_t value = _cpu->reg(_dest);
    reg8_t result = value << 1;
    _cpu->reg(_dest, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, value >> 7);
    return 8;
  }

  std::string name() override {
    return std::string("SLA ") + CPU::reg_name(_dest);
  }

protected:
  CPURegister _dest;
};

class SLA_HLP_Inst : public Instruction {
public:
  SLA_HLP_Inst(CPU *cpu) : Instruction(cpu) {}

  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t value = _cpu->memory(addr);
    reg8_t result = value << 1;
    _cpu->memory(addr, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, value >> 7);
    return 16;
  }

  std::string name() override { return std::string("SLA [HL]"); }
};

//===========================SRA========================
class SRA_REG8_Inst : public Instruction {
public:
  SRA_REG8_Inst(CPU *cpu, CPURegister dest) : Instruction(cpu), _dest(dest) {}

  int internal_exec() override {
    reg8_t value = _cpu->reg(_dest);
    reg8_t result = value >> 1 | (value & 0x80);
    _cpu->reg(_dest, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, value & 0x1);
    return 8;
  }

  std::string name() override {
    return std::string("SRA ") + CPU::reg_name(_dest);
  }

protected:
  CPURegister _dest;
};

class SRA_HLP_Inst : public Instruction {
public:
  SRA_HLP_Inst(CPU *cpu) : Instruction(cpu) {}

  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t value = _cpu->memory(addr);
    reg8_t result = value >> 1 | (value & 0x80);
    _cpu->memory(addr, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, value & 0x1);
    return 16;
  }
  std::string name() override { return std::string("SRA [HL]"); }
};

//===========================SRL========================
class SRL_REG8_Inst : public Instruction {
public:
  SRL_REG8_Inst(CPU *cpu, CPURegister dest) : Instruction(cpu), _dest(dest) {}

  int internal_exec() override {
    reg8_t value = _cpu->reg(_dest);
    reg8_t result = value >> 1;
    _cpu->reg(_dest, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, value & 0x1);
    return 8;
  }

  std::string name() override {
    return std::string("SRL ") + CPU::reg_name(_dest);
  }

protected:
  CPURegister _dest;
};

class SRL_HLP_Inst : public Instruction {
public:
  SRL_HLP_Inst(CPU *cpu) : Instruction(cpu) {}

  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t value = _cpu->memory(addr);
    reg8_t result = value >> 1;
    _cpu->memory(addr, result);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, false);
    _cpu->set_flag(CPUFlagReg::FLAG_CARRY, value & 0x1);
    return 16;
  }
  std::string name() override { return std::string("SRL [HL]"); }
};

//===========================BIT========================
class BIT_B3_REG8_Inst : public Instruction {
public:
  BIT_B3_REG8_Inst(CPU *cpu, d8_t bitPos, CPURegister dest)
      : Instruction(cpu), _bitPos(bitPos), _dest(dest) {}

  int internal_exec() override {
    reg8_t value = _cpu->reg(_dest);
    reg8_t result = value & (1U << _bitPos);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, true);
    return 8;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "BIT " << uint16_t(_bitPos) << "," << CPU::reg_name(_dest);
    return ss.str();
  }

protected:
  d8_t _bitPos;
  CPURegister _dest;
};

class BIT_B3_HLP_Inst : public Instruction {
public:
  BIT_B3_HLP_Inst(CPU *cpu, d8_t bitPos) : Instruction(cpu), _bitPos(bitPos) {}

  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t value = _cpu->memory(addr);
    reg8_t result = value & (1U << _bitPos);

    _cpu->set_flag(CPUFlagReg::FLAG_ZERO, result == 0);
    _cpu->set_flag(CPUFlagReg::FLAG_SUB, false);
    _cpu->set_flag(CPUFlagReg::FLAG_HALF_CARRY, true);
    return 16;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "BIT " << uint16_t(_bitPos) << ",[HL]";
    return ss.str();
  }

protected:
  d8_t _bitPos;
};

class BIT_B3_HLP_SPEC_Inst : public BIT_B3_HLP_Inst {
public:
  BIT_B3_HLP_SPEC_Inst(CPU *cpu, d8_t bitPos) : BIT_B3_HLP_Inst(cpu, bitPos) {}

  int internal_exec() override {
    BIT_B3_HLP_Inst::internal_exec();
    return 12;
  }
};
//===========================RES========================
class RES_B3_REG8_Inst : public Instruction {
public:
  RES_B3_REG8_Inst(CPU *cpu, d8_t bitPos, CPURegister dest)
      : Instruction(cpu), _bitPos(bitPos), _dest(dest) {}

  int internal_exec() override {
    reg8_t value = _cpu->reg(_dest);
    reg8_t result = value & (~(1U << _bitPos));
    _cpu->reg(_dest, result);
    return 8;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "RES " << uint16_t(_bitPos) << "," << CPU::reg_name(_dest);
    return ss.str();
  }

protected:
  d8_t _bitPos;
  CPURegister _dest;
};

class RES_B3_HLP_Inst : public Instruction {
public:
  RES_B3_HLP_Inst(CPU *cpu, d8_t bitPos) : Instruction(cpu), _bitPos(bitPos) {}

  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t value = _cpu->memory(addr);
    reg8_t result = value & (~(1U << _bitPos));
    _cpu->memory(addr, result);
    return 16;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "RES " << uint16_t(_bitPos) << ",[HL]";
    return ss.str();
  }

protected:
  d8_t _bitPos;
};

//===========================SET========================
class SET_B3_REG8_Inst : public Instruction {
public:
  SET_B3_REG8_Inst(CPU *cpu, d8_t bitPos, CPURegister dest)
      : Instruction(cpu), _bitPos(bitPos), _dest(dest) {}

  int internal_exec() override {
    reg8_t value = _cpu->reg(_dest);
    reg8_t result = value | (1U << _bitPos);
    _cpu->reg(_dest, result);
    return 8;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "SET " << uint16_t(_bitPos) << "," << CPU::reg_name(_dest);
    return ss.str();
  }

protected:
  d8_t _bitPos;
  CPURegister _dest;
};

class SET_B3_HLP_Inst : public Instruction {
public:
  SET_B3_HLP_Inst(CPU *cpu, d8_t bitPos) : Instruction(cpu), _bitPos(bitPos) {}

  int internal_exec() override {
    a16_t addr = _cpu->reg(CPURegister::REG_HL);
    reg8_t value = _cpu->memory(addr);
    reg8_t result = value | (1U << _bitPos);
    _cpu->memory(addr, result);
    return 16;
  }

  std::string name() override {
    std::ostringstream ss;
    ss << "SET " << uint16_t(_bitPos) << ",[HL]";
    return ss.str();
  }

protected:
  d8_t _bitPos;
};

} // namespace GB