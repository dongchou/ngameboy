#include "instruction_decoder.h"
#include "inst_arith.h"
#include "inst_bit.h"
#include "inst_jp.h"
#include "inst_ld.h"
#include "inst_misc.h"
#include "inst_stack.h"
#include <iostream>

namespace GB {
#define cast_r8_t(v) static_cast<r8_t>(v)

static inline uint16_t decode_read16(InstructionReader *reader) {
  if (!reader->remain(2)) {
    throw decode_exception("no more bytes readable!");
  }
  uint16_t l = reader->readByte();
  uint16_t h = reader->readByte();
  return h << 8 | l;
}

static inline uint8_t decode_read8(InstructionReader *reader) {
  if (!reader->remain(1)) {
    throw decode_exception("no more bytes readable!");
  }
  return (uint8_t)reader->readByte();
}

InstructionPtr InstructionDecoder::decodeOneCB(InstructionReader *reader) throw(
    decode_exception) {
  if (!reader->remain(1)) {
    return InstructionPtr();
  }

  auto opcode = reader->readByte();
  switch (opcode) {
    //============RLC==========
  case OP_CB_RLC_B:
    return InstructionPtr(new RLC_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_CB_RLC_C:
    return InstructionPtr(new RLC_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_CB_RLC_D:
    return InstructionPtr(new RLC_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_CB_RLC_E:
    return InstructionPtr(new RLC_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_CB_RLC_H:
    return InstructionPtr(new RLC_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_CB_RLC_L:
    return InstructionPtr(new RLC_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_CB_RLC_HLP:
    return InstructionPtr(new RLC_HLP_Inst(_cpu));
  case OP_CB_RLC_A:
    return InstructionPtr(new RLC_REG8_Inst(_cpu, CPURegister::REG_A));
    //============RRC==========
  case OP_CB_RRC_B:
    return InstructionPtr(new RRC_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_CB_RRC_C:
    return InstructionPtr(new RRC_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_CB_RRC_D:
    return InstructionPtr(new RRC_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_CB_RRC_E:
    return InstructionPtr(new RRC_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_CB_RRC_H:
    return InstructionPtr(new RRC_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_CB_RRC_L:
    return InstructionPtr(new RRC_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_CB_RRC_HLP:
    return InstructionPtr(new RRC_HLP_Inst(_cpu));
  case OP_CB_RRC_A:
    return InstructionPtr(new RRC_REG8_Inst(_cpu, CPURegister::REG_A));
    //============RL==========
  case OP_CB_RL_B:
    return InstructionPtr(new RL_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_CB_RL_C:
    return InstructionPtr(new RL_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_CB_RL_D:
    return InstructionPtr(new RL_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_CB_RL_E:
    return InstructionPtr(new RL_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_CB_RL_H:
    return InstructionPtr(new RL_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_CB_RL_L:
    return InstructionPtr(new RL_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_CB_RL_HLP:
    return InstructionPtr(new RL_HLP_Inst(_cpu));
  case OP_CB_RL_A:
    return InstructionPtr(new RL_REG8_Inst(_cpu, CPURegister::REG_A));
    //============RR==========
  case OP_CB_RR_B:
    return InstructionPtr(new RR_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_CB_RR_C:
    return InstructionPtr(new RR_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_CB_RR_D:
    return InstructionPtr(new RR_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_CB_RR_E:
    return InstructionPtr(new RR_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_CB_RR_H:
    return InstructionPtr(new RR_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_CB_RR_L:
    return InstructionPtr(new RR_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_CB_RR_HLP:
    return InstructionPtr(new RR_HLP_Inst(_cpu));
  case OP_CB_RR_A:
    return InstructionPtr(new RR_REG8_Inst(_cpu, CPURegister::REG_A));
    //============SLA==========
  case OP_CB_SLA_B:
    return InstructionPtr(new SLA_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_CB_SLA_C:
    return InstructionPtr(new SLA_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_CB_SLA_D:
    return InstructionPtr(new SLA_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_CB_SLA_E:
    return InstructionPtr(new SLA_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_CB_SLA_H:
    return InstructionPtr(new SLA_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_CB_SLA_L:
    return InstructionPtr(new SLA_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_CB_SLA_HLP:
    return InstructionPtr(new SLA_HLP_Inst(_cpu));
  case OP_CB_SLA_A:
    return InstructionPtr(new SLA_REG8_Inst(_cpu, CPURegister::REG_A));
    //============SRA==========
  case OP_CB_SRA_B:
    return InstructionPtr(new SRA_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_CB_SRA_C:
    return InstructionPtr(new SRA_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_CB_SRA_D:
    return InstructionPtr(new SRA_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_CB_SRA_E:
    return InstructionPtr(new SRA_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_CB_SRA_H:
    return InstructionPtr(new SRA_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_CB_SRA_L:
    return InstructionPtr(new SRA_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_CB_SRA_HLP:
    return InstructionPtr(new SRA_HLP_Inst(_cpu));
  case OP_CB_SRA_A:
    return InstructionPtr(new SRA_REG8_Inst(_cpu, CPURegister::REG_A));
    //============SWAP==========
  case OP_CB_SWAP_B:
    return InstructionPtr(new SWAP_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_CB_SWAP_C:
    return InstructionPtr(new SWAP_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_CB_SWAP_D:
    return InstructionPtr(new SWAP_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_CB_SWAP_E:
    return InstructionPtr(new SWAP_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_CB_SWAP_H:
    return InstructionPtr(new SWAP_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_CB_SWAP_L:
    return InstructionPtr(new SWAP_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_CB_SWAP_HLP:
    return InstructionPtr(new SWAP_HLP_Inst(_cpu));
  case OP_CB_SWAP_A:
    return InstructionPtr(new SWAP_REG8_Inst(_cpu, CPURegister::REG_A));
    //============SRL==========
  case OP_CB_SRL_B:
    return InstructionPtr(new SRL_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_CB_SRL_C:
    return InstructionPtr(new SRL_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_CB_SRL_D:
    return InstructionPtr(new SRL_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_CB_SRL_E:
    return InstructionPtr(new SRL_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_CB_SRL_H:
    return InstructionPtr(new SRL_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_CB_SRL_L:
    return InstructionPtr(new SRL_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_CB_SRL_HLP:
    return InstructionPtr(new SRL_HLP_Inst(_cpu));
  case OP_CB_SRL_A:
    return InstructionPtr(new SRL_REG8_Inst(_cpu, CPURegister::REG_A));
    //============BIT==========
  case OP_CB_BIT_0_B:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 0, CPURegister::REG_B));
  case OP_CB_BIT_0_C:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 0, CPURegister::REG_C));
  case OP_CB_BIT_0_D:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 0, CPURegister::REG_D));
  case OP_CB_BIT_0_E:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 0, CPURegister::REG_E));
  case OP_CB_BIT_0_H:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 0, CPURegister::REG_H));
  case OP_CB_BIT_0_L:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 0, CPURegister::REG_L));
  case OP_CB_BIT_0_HLP:
    return InstructionPtr(new BIT_B3_HLP_SPEC_Inst(_cpu, 0));
  case OP_CB_BIT_0_A:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 0, CPURegister::REG_A));

  case OP_CB_BIT_1_B:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 1, CPURegister::REG_B));
  case OP_CB_BIT_1_C:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 1, CPURegister::REG_C));
  case OP_CB_BIT_1_D:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 1, CPURegister::REG_D));
  case OP_CB_BIT_1_E:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 1, CPURegister::REG_E));
  case OP_CB_BIT_1_H:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 1, CPURegister::REG_H));
  case OP_CB_BIT_1_L:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 1, CPURegister::REG_L));
  case OP_CB_BIT_1_HLP:
    return InstructionPtr(new BIT_B3_HLP_SPEC_Inst(_cpu, 1));
  case OP_CB_BIT_1_A:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 1, CPURegister::REG_A));

  case OP_CB_BIT_2_B:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 2, CPURegister::REG_B));
  case OP_CB_BIT_2_C:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 2, CPURegister::REG_C));
  case OP_CB_BIT_2_D:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 2, CPURegister::REG_D));
  case OP_CB_BIT_2_E:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 2, CPURegister::REG_E));
  case OP_CB_BIT_2_H:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 2, CPURegister::REG_H));
  case OP_CB_BIT_2_L:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 2, CPURegister::REG_L));
  case OP_CB_BIT_2_HLP:
    return InstructionPtr(new BIT_B3_HLP_SPEC_Inst(_cpu, 2));
  case OP_CB_BIT_2_A:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 2, CPURegister::REG_A));
  case OP_CB_BIT_3_B:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 3, CPURegister::REG_B));
  case OP_CB_BIT_3_C:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 3, CPURegister::REG_C));
  case OP_CB_BIT_3_D:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 3, CPURegister::REG_D));
  case OP_CB_BIT_3_E:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 3, CPURegister::REG_E));
  case OP_CB_BIT_3_H:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 3, CPURegister::REG_H));
  case OP_CB_BIT_3_L:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 3, CPURegister::REG_L));
  case OP_CB_BIT_3_HLP:
    return InstructionPtr(new BIT_B3_HLP_SPEC_Inst(_cpu, 3));
  case OP_CB_BIT_3_A:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 3, CPURegister::REG_A));

  case OP_CB_BIT_4_B:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 4, CPURegister::REG_B));
  case OP_CB_BIT_4_C:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 4, CPURegister::REG_C));
  case OP_CB_BIT_4_D:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 4, CPURegister::REG_D));
  case OP_CB_BIT_4_E:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 4, CPURegister::REG_E));
  case OP_CB_BIT_4_H:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 4, CPURegister::REG_H));
  case OP_CB_BIT_4_L:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 4, CPURegister::REG_L));
  case OP_CB_BIT_4_HLP:
    return InstructionPtr(new BIT_B3_HLP_SPEC_Inst(_cpu, 4));
  case OP_CB_BIT_4_A:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 4, CPURegister::REG_A));

  case OP_CB_BIT_5_B:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 5, CPURegister::REG_B));
  case OP_CB_BIT_5_C:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 5, CPURegister::REG_C));
  case OP_CB_BIT_5_D:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 5, CPURegister::REG_D));
  case OP_CB_BIT_5_E:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 5, CPURegister::REG_E));
  case OP_CB_BIT_5_H:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 5, CPURegister::REG_H));
  case OP_CB_BIT_5_L:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 5, CPURegister::REG_L));
  case OP_CB_BIT_5_HLP:
    return InstructionPtr(new BIT_B3_HLP_SPEC_Inst(_cpu, 5));
  case OP_CB_BIT_5_A:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 5, CPURegister::REG_A));

  case OP_CB_BIT_6_B:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 6, CPURegister::REG_B));
  case OP_CB_BIT_6_C:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 6, CPURegister::REG_C));
  case OP_CB_BIT_6_D:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 6, CPURegister::REG_D));
  case OP_CB_BIT_6_E:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 6, CPURegister::REG_E));
  case OP_CB_BIT_6_H:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 6, CPURegister::REG_H));
  case OP_CB_BIT_6_L:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 6, CPURegister::REG_L));
  case OP_CB_BIT_6_HLP:
    return InstructionPtr(new BIT_B3_HLP_SPEC_Inst(_cpu, 6));
  case OP_CB_BIT_6_A:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 6, CPURegister::REG_A));

  case OP_CB_BIT_7_B:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 7, CPURegister::REG_B));
  case OP_CB_BIT_7_C:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 7, CPURegister::REG_C));
  case OP_CB_BIT_7_D:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 7, CPURegister::REG_D));
  case OP_CB_BIT_7_E:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 7, CPURegister::REG_E));
  case OP_CB_BIT_7_H:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 7, CPURegister::REG_H));
  case OP_CB_BIT_7_L:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 7, CPURegister::REG_L));
  case OP_CB_BIT_7_HLP:
    return InstructionPtr(new BIT_B3_HLP_SPEC_Inst(_cpu, 7));
  case OP_CB_BIT_7_A:
    return InstructionPtr(new BIT_B3_REG8_Inst(_cpu, 7, CPURegister::REG_A));

    //============RES==========
  case OP_CB_RES_0_B:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 0, CPURegister::REG_B));
  case OP_CB_RES_0_C:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 0, CPURegister::REG_C));
  case OP_CB_RES_0_D:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 0, CPURegister::REG_D));
  case OP_CB_RES_0_E:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 0, CPURegister::REG_E));
  case OP_CB_RES_0_H:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 0, CPURegister::REG_H));
  case OP_CB_RES_0_L:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 0, CPURegister::REG_L));
  case OP_CB_RES_0_HLP:
    return InstructionPtr(new RES_B3_HLP_Inst(_cpu, 0));
  case OP_CB_RES_0_A:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 0, CPURegister::REG_A));

  case OP_CB_RES_1_B:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 1, CPURegister::REG_B));
  case OP_CB_RES_1_C:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 1, CPURegister::REG_C));
  case OP_CB_RES_1_D:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 1, CPURegister::REG_D));
  case OP_CB_RES_1_E:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 1, CPURegister::REG_E));
  case OP_CB_RES_1_H:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 1, CPURegister::REG_H));
  case OP_CB_RES_1_L:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 1, CPURegister::REG_L));
  case OP_CB_RES_1_HLP:
    return InstructionPtr(new RES_B3_HLP_Inst(_cpu, 1));
  case OP_CB_RES_1_A:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 1, CPURegister::REG_A));

  case OP_CB_RES_2_B:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 2, CPURegister::REG_B));
  case OP_CB_RES_2_C:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 2, CPURegister::REG_C));
  case OP_CB_RES_2_D:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 2, CPURegister::REG_D));
  case OP_CB_RES_2_E:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 2, CPURegister::REG_E));
  case OP_CB_RES_2_H:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 2, CPURegister::REG_H));
  case OP_CB_RES_2_L:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 2, CPURegister::REG_L));
  case OP_CB_RES_2_HLP:
    return InstructionPtr(new RES_B3_HLP_Inst(_cpu, 2));
  case OP_CB_RES_2_A:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 2, CPURegister::REG_A));
  case OP_CB_RES_3_B:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 3, CPURegister::REG_B));
  case OP_CB_RES_3_C:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 3, CPURegister::REG_C));
  case OP_CB_RES_3_D:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 3, CPURegister::REG_D));
  case OP_CB_RES_3_E:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 3, CPURegister::REG_E));
  case OP_CB_RES_3_H:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 3, CPURegister::REG_H));
  case OP_CB_RES_3_L:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 3, CPURegister::REG_L));
  case OP_CB_RES_3_HLP:
    return InstructionPtr(new RES_B3_HLP_Inst(_cpu, 3));
  case OP_CB_RES_3_A:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 3, CPURegister::REG_A));

  case OP_CB_RES_4_B:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 4, CPURegister::REG_B));
  case OP_CB_RES_4_C:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 4, CPURegister::REG_C));
  case OP_CB_RES_4_D:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 4, CPURegister::REG_D));
  case OP_CB_RES_4_E:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 4, CPURegister::REG_E));
  case OP_CB_RES_4_H:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 4, CPURegister::REG_H));
  case OP_CB_RES_4_L:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 4, CPURegister::REG_L));
  case OP_CB_RES_4_HLP:
    return InstructionPtr(new RES_B3_HLP_Inst(_cpu, 4));
  case OP_CB_RES_4_A:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 4, CPURegister::REG_A));

  case OP_CB_RES_5_B:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 5, CPURegister::REG_B));
  case OP_CB_RES_5_C:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 5, CPURegister::REG_C));
  case OP_CB_RES_5_D:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 5, CPURegister::REG_D));
  case OP_CB_RES_5_E:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 5, CPURegister::REG_E));
  case OP_CB_RES_5_H:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 5, CPURegister::REG_H));
  case OP_CB_RES_5_L:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 5, CPURegister::REG_L));
  case OP_CB_RES_5_HLP:
    return InstructionPtr(new RES_B3_HLP_Inst(_cpu, 5));
  case OP_CB_RES_5_A:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 5, CPURegister::REG_A));

  case OP_CB_RES_6_B:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 6, CPURegister::REG_B));
  case OP_CB_RES_6_C:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 6, CPURegister::REG_C));
  case OP_CB_RES_6_D:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 6, CPURegister::REG_D));
  case OP_CB_RES_6_E:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 6, CPURegister::REG_E));
  case OP_CB_RES_6_H:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 6, CPURegister::REG_H));
  case OP_CB_RES_6_L:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 6, CPURegister::REG_L));
  case OP_CB_RES_6_HLP:
    return InstructionPtr(new RES_B3_HLP_Inst(_cpu, 6));
  case OP_CB_RES_6_A:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 6, CPURegister::REG_A));

  case OP_CB_RES_7_B:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 7, CPURegister::REG_B));
  case OP_CB_RES_7_C:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 7, CPURegister::REG_C));
  case OP_CB_RES_7_D:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 7, CPURegister::REG_D));
  case OP_CB_RES_7_E:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 7, CPURegister::REG_E));
  case OP_CB_RES_7_H:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 7, CPURegister::REG_H));
  case OP_CB_RES_7_L:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 7, CPURegister::REG_L));
  case OP_CB_RES_7_HLP:
    return InstructionPtr(new RES_B3_HLP_Inst(_cpu, 7));
  case OP_CB_RES_7_A:
    return InstructionPtr(new RES_B3_REG8_Inst(_cpu, 7, CPURegister::REG_A));
    //============SET==========
  case OP_CB_SET_0_B:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 0, CPURegister::REG_B));
  case OP_CB_SET_0_C:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 0, CPURegister::REG_C));
  case OP_CB_SET_0_D:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 0, CPURegister::REG_D));
  case OP_CB_SET_0_E:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 0, CPURegister::REG_E));
  case OP_CB_SET_0_H:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 0, CPURegister::REG_H));
  case OP_CB_SET_0_L:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 0, CPURegister::REG_L));
  case OP_CB_SET_0_HLP:
    return InstructionPtr(new SET_B3_HLP_Inst(_cpu, 0));
  case OP_CB_SET_0_A:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 0, CPURegister::REG_A));

  case OP_CB_SET_1_B:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 1, CPURegister::REG_B));
  case OP_CB_SET_1_C:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 1, CPURegister::REG_C));
  case OP_CB_SET_1_D:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 1, CPURegister::REG_D));
  case OP_CB_SET_1_E:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 1, CPURegister::REG_E));
  case OP_CB_SET_1_H:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 1, CPURegister::REG_H));
  case OP_CB_SET_1_L:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 1, CPURegister::REG_L));
  case OP_CB_SET_1_HLP:
    return InstructionPtr(new SET_B3_HLP_Inst(_cpu, 1));
  case OP_CB_SET_1_A:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 1, CPURegister::REG_A));

  case OP_CB_SET_2_B:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 2, CPURegister::REG_B));
  case OP_CB_SET_2_C:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 2, CPURegister::REG_C));
  case OP_CB_SET_2_D:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 2, CPURegister::REG_D));
  case OP_CB_SET_2_E:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 2, CPURegister::REG_E));
  case OP_CB_SET_2_H:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 2, CPURegister::REG_H));
  case OP_CB_SET_2_L:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 2, CPURegister::REG_L));
  case OP_CB_SET_2_HLP:
    return InstructionPtr(new SET_B3_HLP_Inst(_cpu, 2));
  case OP_CB_SET_2_A:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 2, CPURegister::REG_A));
  case OP_CB_SET_3_B:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 3, CPURegister::REG_B));
  case OP_CB_SET_3_C:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 3, CPURegister::REG_C));
  case OP_CB_SET_3_D:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 3, CPURegister::REG_D));
  case OP_CB_SET_3_E:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 3, CPURegister::REG_E));
  case OP_CB_SET_3_H:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 3, CPURegister::REG_H));
  case OP_CB_SET_3_L:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 3, CPURegister::REG_L));
  case OP_CB_SET_3_HLP:
    return InstructionPtr(new SET_B3_HLP_Inst(_cpu, 3));
  case OP_CB_SET_3_A:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 3, CPURegister::REG_A));

  case OP_CB_SET_4_B:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 4, CPURegister::REG_B));
  case OP_CB_SET_4_C:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 4, CPURegister::REG_C));
  case OP_CB_SET_4_D:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 4, CPURegister::REG_D));
  case OP_CB_SET_4_E:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 4, CPURegister::REG_E));
  case OP_CB_SET_4_H:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 4, CPURegister::REG_H));
  case OP_CB_SET_4_L:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 4, CPURegister::REG_L));
  case OP_CB_SET_4_HLP:
    return InstructionPtr(new SET_B3_HLP_Inst(_cpu, 4));
  case OP_CB_SET_4_A:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 4, CPURegister::REG_A));

  case OP_CB_SET_5_B:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 5, CPURegister::REG_B));
  case OP_CB_SET_5_C:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 5, CPURegister::REG_C));
  case OP_CB_SET_5_D:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 5, CPURegister::REG_D));
  case OP_CB_SET_5_E:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 5, CPURegister::REG_E));
  case OP_CB_SET_5_H:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 5, CPURegister::REG_H));
  case OP_CB_SET_5_L:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 5, CPURegister::REG_L));
  case OP_CB_SET_5_HLP:
    return InstructionPtr(new SET_B3_HLP_Inst(_cpu, 5));
  case OP_CB_SET_5_A:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 5, CPURegister::REG_A));

  case OP_CB_SET_6_B:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 6, CPURegister::REG_B));
  case OP_CB_SET_6_C:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 6, CPURegister::REG_C));
  case OP_CB_SET_6_D:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 6, CPURegister::REG_D));
  case OP_CB_SET_6_E:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 6, CPURegister::REG_E));
  case OP_CB_SET_6_H:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 6, CPURegister::REG_H));
  case OP_CB_SET_6_L:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 6, CPURegister::REG_L));
  case OP_CB_SET_6_HLP:
    return InstructionPtr(new SET_B3_HLP_Inst(_cpu, 6));
  case OP_CB_SET_6_A:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 6, CPURegister::REG_A));

  case OP_CB_SET_7_B:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 7, CPURegister::REG_B));
  case OP_CB_SET_7_C:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 7, CPURegister::REG_C));
  case OP_CB_SET_7_D:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 7, CPURegister::REG_D));
  case OP_CB_SET_7_E:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 7, CPURegister::REG_E));
  case OP_CB_SET_7_H:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 7, CPURegister::REG_H));
  case OP_CB_SET_7_L:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 7, CPURegister::REG_L));
  case OP_CB_SET_7_HLP:
    return InstructionPtr(new SET_B3_HLP_Inst(_cpu, 7));
  case OP_CB_SET_7_A:
    return InstructionPtr(new SET_B3_REG8_Inst(_cpu, 7, CPURegister::REG_A));
  default:
    break;
  }

  throw decode_exception("unknown cb instruction ");
}

InstructionPtr InstructionDecoder::decodeOne(InstructionReader *reader) throw(
    decode_exception) {
  if (!reader->remain(1)) {
    return InstructionPtr();
  }

  auto read16 = [&] { return decode_read16(reader); };
  auto read8 = [&] { return decode_read8(reader); };

  auto opcode = reader->readByte();
  switch (opcode) {
  case OP_NOP:
    return InstructionPtr(new Nop_Inst(_cpu));
  case OP_LD_BC_D16:
    return InstructionPtr(
        new LD_REG16_D16_Inst(_cpu, CPURegister::REG_BC, (d16_t)read16()));
  case OP_LD_BCP_A:
    return InstructionPtr(new LD_REG16P_A_Inst(_cpu, CPURegister::REG_BC));
  case OP_INC_BC:
    return InstructionPtr(new INC_REG16_Inst(_cpu, CPURegister::REG_BC));
  case OP_INC_B:
    return InstructionPtr(new INC_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_DEC_B:
    return InstructionPtr(new DEC_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_LD_B_D8:
    return InstructionPtr(
        new LD_REG8_D8_Inst(_cpu, CPURegister::REG_B, (d8_t)read8()));
  case OP_RLCA:
    return InstructionPtr(new RLCA_Inst(_cpu));
  case OP_LD_A16P_SP:
    return InstructionPtr(new LD_D16P_SP_Inst(_cpu, (d16_t)read16()));
  case OP_ADD_HL_BC:
    return InstructionPtr(new ADD_HL_REG16_Inst(_cpu, CPURegister::REG_BC));
  case OP_LD_A_BCP:
    return InstructionPtr(new LD_A_REG16P_Inst(_cpu, CPURegister::REG_BC));
  case OP_DEC_BC:
    return InstructionPtr(new DEC_REG16_Inst(_cpu, CPURegister::REG_BC));
  case OP_INC_C:
    return InstructionPtr(new INC_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_DEC_C:
    return InstructionPtr(new DEC_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_LD_C_D8:
    return InstructionPtr(
        new LD_REG8_D8_Inst(_cpu, CPURegister::REG_C, (d8_t)read8()));
  case OP_RRCA:
    return InstructionPtr(new RRCA_Inst(_cpu));
  case OP_STOP:
    return InstructionPtr(new Stop_Inst(_cpu));
  case OP_LD_DE_D16:
    return InstructionPtr(
        new LD_REG16_D16_Inst(_cpu, CPURegister::REG_DE, (d16_t)read16()));
  case OP_LD_DEP_A:
    return InstructionPtr(new LD_REG16P_A_Inst(_cpu, CPURegister::REG_DE));
  case OP_INC_DE:
    return InstructionPtr(new INC_REG16_Inst(_cpu, CPURegister::REG_DE));
  case OP_INC_D:
    return InstructionPtr(new INC_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_DEC_D:
    return InstructionPtr(new DEC_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_LD_D_D8:
    return InstructionPtr(
        new LD_REG8_D8_Inst(_cpu, CPURegister::REG_D, (d8_t)read8()));
  case OP_RLA:
    return InstructionPtr(new RLA_Inst(_cpu));
  case OP_JR_R8:
    return InstructionPtr(new JR_R8_Inst(_cpu, (r8_t)read8()));
  case OP_ADD_HL_DE:
    return InstructionPtr(new ADD_HL_REG16_Inst(_cpu, CPURegister::REG_DE));
  case OP_LD_A_DEP:
    return InstructionPtr(new LD_A_REG16P_Inst(_cpu, CPURegister::REG_DE));
  case OP_DEC_DE:
    return InstructionPtr(new DEC_REG16_Inst(_cpu, CPURegister::REG_DE));
  case OP_INC_E:
    return InstructionPtr(new INC_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_DEC_E:
    return InstructionPtr(new DEC_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_LD_E_D8:
    return InstructionPtr(
        new LD_REG8_D8_Inst(_cpu, CPURegister::REG_E, (d8_t)read8()));
  case OP_RRA:
    return InstructionPtr(new RRA_Inst(_cpu));
  case OP_JR_NZ_R8:
    return InstructionPtr(
        new JR_CC_R8_Inst(_cpu, JumpCC::CC_NZ, (r8_t)read8()));
  case OP_LD_HL_D16:
    return InstructionPtr(
        new LD_REG16_D16_Inst(_cpu, CPURegister::REG_HL, (d16_t)read16()));
  case OP_LDI_HL_A:
    return InstructionPtr(new LDI_HLP_A_Inst(_cpu));
  case OP_INC_HL:
    return InstructionPtr(new INC_REG16_Inst(_cpu, CPURegister::REG_HL));
  case OP_INC_H:
    return InstructionPtr(new INC_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_DEC_H:
    return InstructionPtr(new DEC_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_LD_H_d8:
    return InstructionPtr(
        new LD_REG8_D8_Inst(_cpu, CPURegister::REG_H, (d8_t)read8()));
  case OP_DAA:
    return InstructionPtr(new DAA_Inst(_cpu));
  case OP_JR_Z_R8:
    return InstructionPtr(new JR_CC_R8_Inst(_cpu, JumpCC::CC_Z, (r8_t)read8()));
  case OP_ADD_HL_HL:
    return InstructionPtr(new ADD_HL_REG16_Inst(_cpu, CPURegister::REG_HL));
  case OP_LDI_A_HL:
    return InstructionPtr(new LDI_A_HLP_Inst(_cpu));
  case OP_DEC_HL:
    return InstructionPtr(new DEC_REG16_Inst(_cpu, CPURegister::REG_HL));
  case OP_INC_L:
    return InstructionPtr(new INC_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_DEC_L:
    return InstructionPtr(new DEC_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_LD_L_D8:
    return InstructionPtr(
        new LD_REG8_D8_Inst(_cpu, CPURegister::REG_L, (d8_t)read8()));
  case OP_CPL:
    return InstructionPtr(new CPL_Inst(_cpu));
  case OP_JR_NC_R8:
    return InstructionPtr(
        new JR_CC_R8_Inst(_cpu, JumpCC::CC_NC, (r8_t)read8()));
  case OP_LD_SP_D16:
    return InstructionPtr(new LD_SP_D16_Inst(_cpu, (d16_t)read16()));
  case OP_LDD_HLP_A:
    return InstructionPtr(new LDD_HLP_A_Inst(_cpu));
  case OP_INC_SP:
    return InstructionPtr(new INC_SP_Inst(_cpu));
  case OP_INC_HLP:
    return InstructionPtr(new INC_HLP_Inst(_cpu));
  case OP_DEC_HLP:
    return InstructionPtr(new DEC_HLP_Inst(_cpu));
  case OP_LD_HLP_D8:
    return InstructionPtr(new LD_HLP_D8_Inst(_cpu, (d8_t)read8()));
  case OP_SCF:
    return InstructionPtr(new SCF_Inst(_cpu));
  case OP_JR_C_R8:
    return InstructionPtr(new JR_CC_R8_Inst(_cpu, JumpCC::CC_C, (r8_t)read8()));
  case OP_ADD_HL_SP:
    return InstructionPtr(new ADD_HL_SP_Inst(_cpu));
  case OP_LDD_A_HLP:
    return InstructionPtr(new LDD_A_HLP_Inst(_cpu));
  case OP_DEC_SP:
    return InstructionPtr(new DEC_SP_Inst(_cpu));
  case OP_INC_A:
    return InstructionPtr(new INC_REG8_Inst(_cpu, CPURegister::REG_A));
  case OP_DEC_A:
    return InstructionPtr(new DEC_REG8_Inst(_cpu, CPURegister::REG_A));
  case OP_LD_A_D8:
    return InstructionPtr(
        new LD_REG8_D8_Inst(_cpu, CPURegister::REG_A, (d8_t)read8()));
  case OP_CCF:
    return InstructionPtr(new CCF_Inst(_cpu));
  case OP_LD_B_B:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_B, CPURegister::REG_B));
  case OP_LD_B_C:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_B, CPURegister::REG_C));
  case OP_LD_B_D:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_B, CPURegister::REG_D));
  case OP_LD_B_E:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_B, CPURegister::REG_E));
  case OP_LD_B_H:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_B, CPURegister::REG_H));
  case OP_LD_B_L:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_B, CPURegister::REG_L));
  case OP_LD_B_HLP:
    return InstructionPtr(new LD_REG8_HLP_Inst(_cpu, CPURegister::REG_B));
  case OP_LD_B_A:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_B, CPURegister::REG_A));
    //=====
  case OP_LD_C_B:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_C, CPURegister::REG_B));
  case OP_LD_C_C:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_C, CPURegister::REG_C));
  case OP_LD_C_D:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_C, CPURegister::REG_D));
  case OP_LD_C_E:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_C, CPURegister::REG_E));
  case OP_LD_C_H:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_C, CPURegister::REG_H));
  case OP_LD_C_L:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_C, CPURegister::REG_L));
  case OP_LD_C_HLP:
    return InstructionPtr(new LD_REG8_HLP_Inst(_cpu, CPURegister::REG_C));
  case OP_LD_C_A:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_C, CPURegister::REG_A));
    //=====
  case OP_LD_D_B:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_D, CPURegister::REG_B));
  case OP_LD_D_C:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_D, CPURegister::REG_C));
  case OP_LD_D_D:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_D, CPURegister::REG_D));
  case OP_LD_D_E:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_D, CPURegister::REG_E));
  case OP_LD_D_H:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_D, CPURegister::REG_H));
  case OP_LD_D_L:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_D, CPURegister::REG_L));
  case OP_LD_D_HLP:
    return InstructionPtr(new LD_REG8_HLP_Inst(_cpu, CPURegister::REG_D));
  case OP_LD_D_A:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_D, CPURegister::REG_A));
    //=====eeee
  case OP_LD_E_B:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_E, CPURegister::REG_B));
  case OP_LD_E_C:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_E, CPURegister::REG_C));
  case OP_LD_E_D:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_E, CPURegister::REG_D));
  case OP_LD_E_E:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_E, CPURegister::REG_E));
  case OP_LD_E_H:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_E, CPURegister::REG_H));
  case OP_LD_E_L:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_E, CPURegister::REG_L));
  case OP_LD_E_HLP:
    return InstructionPtr(new LD_REG8_HLP_Inst(_cpu, CPURegister::REG_E));
  case OP_LD_E_A:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_E, CPURegister::REG_A));
    //=====h
  case OP_LD_H_B:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_H, CPURegister::REG_B));
  case OP_LD_H_C:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_H, CPURegister::REG_C));
  case OP_LD_H_D:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_H, CPURegister::REG_D));
  case OP_LD_H_E:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_H, CPURegister::REG_E));
  case OP_LD_H_H:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_H, CPURegister::REG_H));
  case OP_LD_H_L:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_H, CPURegister::REG_L));
  case OP_LD_H_HLP:
    return InstructionPtr(new LD_REG8_HLP_Inst(_cpu, CPURegister::REG_H));
  case OP_LD_H_A:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_H, CPURegister::REG_A));
    //=====L
  case OP_LD_L_B:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_L, CPURegister::REG_B));
  case OP_LD_L_C:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_L, CPURegister::REG_C));
  case OP_LD_L_D:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_L, CPURegister::REG_D));
  case OP_LD_L_E:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_L, CPURegister::REG_E));
  case OP_LD_L_H:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_L, CPURegister::REG_H));
  case OP_LD_L_L:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_L, CPURegister::REG_L));
  case OP_LD_L_HLP:
    return InstructionPtr(new LD_REG8_HLP_Inst(_cpu, CPURegister::REG_L));
  case OP_LD_L_A:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_L, CPURegister::REG_A));
  case OP_LD_HLP_B:
    return InstructionPtr(new LD_HLP_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_LD_HLP_C:
    return InstructionPtr(new LD_HLP_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_LD_HLP_D:
    return InstructionPtr(new LD_HLP_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_LD_HLP_E:
    return InstructionPtr(new LD_HLP_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_LD_HLP_H:
    return InstructionPtr(new LD_HLP_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_LD_HLP_L:
    return InstructionPtr(new LD_HLP_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_HALT:
    return InstructionPtr(new Halt_Inst(_cpu));
  case OP_LD_HLP_A:
    return InstructionPtr(new LD_HLP_REG8_Inst(_cpu, CPURegister::REG_A));
    //=========A
  case OP_LD_A_B:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_A, CPURegister::REG_B));
  case OP_LD_A_C:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_A, CPURegister::REG_C));
  case OP_LD_A_D:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_A, CPURegister::REG_D));
  case OP_LD_A_E:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_A, CPURegister::REG_E));
  case OP_LD_A_H:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_A, CPURegister::REG_H));
  case OP_LD_A_L:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_A, CPURegister::REG_L));
  case OP_LD_A_HLP:
    return InstructionPtr(new LD_REG8_HLP_Inst(_cpu, CPURegister::REG_A));
  case OP_LD_A_A:
    return InstructionPtr(
        new LD_REG8_REG8_Inst(_cpu, CPURegister::REG_A, CPURegister::REG_A));
  case OP_ADD_A_B:
    return InstructionPtr(new ADD_A_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_ADD_A_C:
    return InstructionPtr(new ADD_A_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_ADD_A_D:
    return InstructionPtr(new ADD_A_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_ADD_A_E:
    return InstructionPtr(new ADD_A_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_ADD_A_H:
    return InstructionPtr(new ADD_A_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_ADD_A_L:
    return InstructionPtr(new ADD_A_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_ADD_A_HLP:
    return InstructionPtr(new ADD_A_HLP_Inst(_cpu));
  case OP_ADD_A_A:
    return InstructionPtr(new ADD_A_REG8_Inst(_cpu, CPURegister::REG_A));
  //===================ADC
  case OP_ADC_A_B:
    return InstructionPtr(new ADC_A_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_ADC_A_C:
    return InstructionPtr(new ADC_A_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_ADC_A_D:
    return InstructionPtr(new ADC_A_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_ADC_A_E:
    return InstructionPtr(new ADC_A_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_ADC_A_H:
    return InstructionPtr(new ADC_A_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_ADC_A_L:
    return InstructionPtr(new ADC_A_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_ADC_A_HLP:
    return InstructionPtr(new ADC_A_HLP_Inst(_cpu));
  case OP_ADC_A_A:
    return InstructionPtr(new ADC_A_REG8_Inst(_cpu, CPURegister::REG_A));
    //======================SUB
  case OP_SUB_B:
    return InstructionPtr(new SUB_A_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_SUB_C:
    return InstructionPtr(new SUB_A_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_SUB_D:
    return InstructionPtr(new SUB_A_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_SUB_E:
    return InstructionPtr(new SUB_A_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_SUB_H:
    return InstructionPtr(new SUB_A_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_SUB_L:
    return InstructionPtr(new SUB_A_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_SUB_HLP:
    return InstructionPtr(new SUB_A_HLP_Inst(_cpu));
  case OP_SUB_A:
    return InstructionPtr(new SUB_A_REG8_Inst(_cpu, CPURegister::REG_A));
    //======================SBC
  case OP_SBC_A_B:
    return InstructionPtr(new SBC_A_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_SBC_A_C:
    return InstructionPtr(new SBC_A_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_SBC_A_D:
    return InstructionPtr(new SBC_A_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_SBC_A_E:
    return InstructionPtr(new SBC_A_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_SBC_A_H:
    return InstructionPtr(new SBC_A_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_SBC_A_L:
    return InstructionPtr(new SBC_A_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_SBC_A_HLP:
    return InstructionPtr(new SBC_A_HLP_Inst(_cpu));
  case OP_SBC_A_A:
    return InstructionPtr(new SBC_A_REG8_Inst(_cpu, CPURegister::REG_A));

    //=====
  case OP_AND_B:
    return InstructionPtr(new AND_A_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_AND_C:
    return InstructionPtr(new AND_A_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_AND_D:
    return InstructionPtr(new AND_A_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_AND_E:
    return InstructionPtr(new AND_A_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_AND_H:
    return InstructionPtr(new AND_A_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_AND_L:
    return InstructionPtr(new AND_A_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_AND_HLP:
    return InstructionPtr(new AND_A_HLP_Inst(_cpu));
  case OP_AND_A:
    return InstructionPtr(new AND_A_REG8_Inst(_cpu, CPURegister::REG_A));
    //=====xor
  case OP_XOR_B:
    return InstructionPtr(new XOR_A_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_XOR_C:
    return InstructionPtr(new XOR_A_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_XOR_D:
    return InstructionPtr(new XOR_A_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_XOR_E:
    return InstructionPtr(new XOR_A_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_XOR_H:
    return InstructionPtr(new XOR_A_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_XOR_L:
    return InstructionPtr(new XOR_A_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_XOR_HLP:
    return InstructionPtr(new XOR_A_HLP_Inst(_cpu));
  case OP_XOR_A:
    return InstructionPtr(new XOR_A_REG8_Inst(_cpu, CPURegister::REG_A));
    //=====or
  case OP_OR_B:
    return InstructionPtr(new OR_A_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_OR_C:
    return InstructionPtr(new OR_A_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_OR_D:
    return InstructionPtr(new OR_A_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_OR_E:
    return InstructionPtr(new OR_A_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_OR_H:
    return InstructionPtr(new OR_A_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_OR_L:
    return InstructionPtr(new OR_A_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_OR_HLP:
    return InstructionPtr(new OR_A_HLP_Inst(_cpu));
  case OP_OR_A:
    return InstructionPtr(new OR_A_REG8_Inst(_cpu, CPURegister::REG_A));
    //=====cp
  case OP_CP_B:
    return InstructionPtr(new CP_A_REG8_Inst(_cpu, CPURegister::REG_B));
  case OP_CP_C:
    return InstructionPtr(new CP_A_REG8_Inst(_cpu, CPURegister::REG_C));
  case OP_CP_D:
    return InstructionPtr(new CP_A_REG8_Inst(_cpu, CPURegister::REG_D));
  case OP_CP_E:
    return InstructionPtr(new CP_A_REG8_Inst(_cpu, CPURegister::REG_E));
  case OP_CP_H:
    return InstructionPtr(new CP_A_REG8_Inst(_cpu, CPURegister::REG_H));
  case OP_CP_L:
    return InstructionPtr(new CP_A_REG8_Inst(_cpu, CPURegister::REG_L));
  case OP_CP_HLP:
    return InstructionPtr(new CP_A_HLP_Inst(_cpu));
  case OP_CP_A:
    return InstructionPtr(new CP_A_REG8_Inst(_cpu, CPURegister::REG_A));
    //===========
  case OP_RET_NZ:
    return InstructionPtr(new RET_CC_Inst(_cpu, JumpCC::CC_NZ));
  case OP_POP_BC:
    return InstructionPtr(new POP_REG16_Inst(_cpu, CPURegister::REG_BC));
  case OP_JP_NZ_A16:
    return InstructionPtr(
        new JP_CC_A16_Inst(_cpu, JumpCC::CC_NZ, (a16_t)read16()));
  case OP_JP_A16:
    return InstructionPtr(new JP_A16_Inst(_cpu, (a16_t)read16()));
  case OP_CALL_NZ_A16:
    return InstructionPtr(
        new CALL_CC_A16_Inst(_cpu, JumpCC::CC_NZ, (a16_t)read16()));
  case OP_PUSH_BC:
    return InstructionPtr(new PUSH_REG16_Inst(_cpu, CPURegister::REG_BC));
  case OP_ADD_A_D8:
    return InstructionPtr(new ADD_A_D8_Inst(_cpu, (d8_t)read8()));
  case OP_RST_00H:
    return InstructionPtr(new RST_Inst(_cpu, vec_t(0x00)));
  case OP_RET_Z:
    return InstructionPtr(new RET_CC_Inst(_cpu, JumpCC::CC_Z));
  case OP_RET:
    return InstructionPtr(new RET_Inst(_cpu));
  case OP_JP_Z_A16:
    return InstructionPtr(
        new JP_CC_A16_Inst(_cpu, JumpCC::CC_Z, (a16_t)read16()));
  case OP_PREFIX_CB:
    return decodeOneCB(reader);
  case OP_CALL_Z_A16:
    return InstructionPtr(
        new CALL_CC_A16_Inst(_cpu, JumpCC::CC_Z, (a16_t)read16()));
  case OP_CALL_A16:
    return InstructionPtr(new CALL_A16_Inst(_cpu, (a16_t)read16()));
  case OP_ADC_A_D8:
    return InstructionPtr(new ADC_A_D8_Inst(_cpu, (d8_t)read8()));
  case OP_RST_08H:
    return InstructionPtr(new RST_Inst(_cpu, vec_t(0x08)));
  case OP_RET_NC:
    return InstructionPtr(new RET_CC_Inst(_cpu, JumpCC::CC_NC));
  case OP_POP_DE:
    return InstructionPtr(new POP_REG16_Inst(_cpu, CPURegister::REG_DE));
  case OP_JP_NC_A16:
    return InstructionPtr(new JP_CC_A16_Inst(_cpu, JumpCC::CC_NC, read16()));
  case OP_UNUSABLE_1:
    break;
  case OP_CALL_NC_A16:
    return InstructionPtr(new CALL_CC_A16_Inst(_cpu, JumpCC::CC_NC, read16()));
  case OP_PUSH_DE:
    return InstructionPtr(new PUSH_REG16_Inst(_cpu, CPURegister::REG_DE));
  case OP_SUB_D8:
    return InstructionPtr(new SUB_A_D8_Inst(_cpu, read8()));
  case OP_RST_10H:
    return InstructionPtr(new RST_Inst(_cpu, vec_t(0x10)));
  case OP_RET_C:
    return InstructionPtr(new RET_CC_Inst(_cpu, JumpCC::CC_C));
  case OP_RETI:
    return InstructionPtr(new RETI_Inst(_cpu));
  case OP_JP_C_A16:
    return InstructionPtr(new JP_CC_A16_Inst(_cpu, JumpCC::CC_C, read16()));
  case OP_UNUSABLE_2:
    break;
  case OP_CALL_C_A16:
    return InstructionPtr(new CALL_CC_A16_Inst(_cpu, JumpCC::CC_C, read16()));
  case OP_UNUSABLE_3:
    break;
  case OP_SBC_A_D8:
    return InstructionPtr(new SBC_A_D8_Inst(_cpu, read8()));
  case OP_RST_18H:
    return InstructionPtr(new RST_Inst(_cpu, vec_t(0x18)));
  case OP_LDH_A8_A:
    return InstructionPtr(new LDH_A8_A_Inst(_cpu, read8()));
  case OP_POP_HL:
    return InstructionPtr(new POP_REG16_Inst(_cpu, CPURegister::REG_HL));
  case OP_LD_CP_A:
    return InstructionPtr(new LD_CP_A_Inst(_cpu));
  case OP_UNUSABLE_4:
    break;
  case OP_UNUSABLE_5:
    break;
  case OP_PUSH_HL:
    return InstructionPtr(new PUSH_REG16_Inst(_cpu, CPURegister::REG_HL));
  case OP_AND_D8:
    return InstructionPtr(new AND_A_D8_Inst(_cpu, read8()));
  case OP_RST_20H:
    return InstructionPtr(new RST_Inst(_cpu, vec_t(0x20)));
  case OP_ADD_SP_R8:
    return InstructionPtr(new ADD_SP_R8_Inst(_cpu, cast_r8_t(read8())));
  case OP_JP_HLP:
    return InstructionPtr(new JP_HLP_Inst(_cpu));
  case OP_LD_A16_A:
    return InstructionPtr(new LD_A16_A_Inst(_cpu, read16()));
  case OP_UNUSABLE_6:
    break;
  case OP_UNUSABLE_7:
    break;
  case OP_UNUSABLE_8:
    break;
  case OP_XOR_D8:
    return InstructionPtr(new XOR_A_D8_Inst(_cpu, read8()));
  case OP_RST_28H:
    return InstructionPtr(new RST_Inst(_cpu, vec_t(0x28)));
  case OP_LDH_A_A8:
    return InstructionPtr(new LDH_A_A8_Inst(_cpu, read8()));
  case OP_POP_AF:
    return InstructionPtr(new POP_REG16_Inst(_cpu, CPURegister::REG_AF));
  case OP_LD_A_CP:
    return InstructionPtr(new LD_A_CP_Inst(_cpu));
  case OP_DI:
    return InstructionPtr(new DI_Inst(_cpu));
  case OP_UNUSABLE_9:
    break;
  case OP_PUSH_AF:
    return InstructionPtr(new PUSH_REG16_Inst(_cpu, CPURegister::REG_AF));
  case OP_OR_D8:
    return InstructionPtr(new OR_A_D8_Inst(_cpu, read8()));
  case OP_RST_30H:
    return InstructionPtr(new RST_Inst(_cpu, vec_t(0x30)));
  case OP_LDHL_SP_R8:
    return InstructionPtr(new LDHL_SP_R8_Inst(_cpu, read8()));
  case OP_LD_SP_HL:
    return InstructionPtr(new LD_SP_HL_Inst(_cpu));
  case OP_LD_A_A16:
    return InstructionPtr(new LD_A_A16_Inst(_cpu, read16()));
  case OP_EI:
    return InstructionPtr(new EI_Inst(_cpu));
  case OP_UNUSABLE_10:
    break;
  case OP_UNUSABLE_11:
    break;
  case OP_CP_D8:
    return InstructionPtr(new CP_A_D8_Inst(_cpu, read8()));
  case OP_RST_38H:
    return InstructionPtr(new RST_Inst(_cpu, vec_t(0x38)));
  default:
    break;
  }

  throw decode_exception("unknown instruction ");
}

InstructionPtr InstructionDecoder::decode() throw(decode_exception) {
  InstructionReader reader(_cpu);
  return decodeOne(&reader);
}
} // namespace GB