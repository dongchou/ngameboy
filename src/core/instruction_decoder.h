#pragma once

#include "cpu.h"
#include "instruction.h"
#include "instruction_opcode.h"
#include <memory>
#include <string>
#include <vector>

namespace GB {

typedef std::unique_ptr<Instruction> InstructionPtr;

class decode_exception : public std::exception {
public:
  explicit decode_exception(const std::string &reason) : _reason(reason) {}
  virtual const char *what() const noexcept { return _reason.c_str(); }

private:
  std::string _reason;
};

class InstructionReader final : public non_copyable {
public:
  InstructionReader(CPU *cpu) : cpu_(cpu) {
    inst_offset_ = cpu_->reg(CPURegister::REG_PC);
  }
  ~InstructionReader() { cpu_->reg(CPURegister::REG_PC, inst_offset_); }
  byte readByte() { return cpu_->memory(inst_offset_++); }
  bool remain(size_t n) const { return true; }

private:
  CPU *cpu_;
  int inst_offset_;
};

class InstructionDecoder final {
public:
  InstructionDecoder(CPU *cpu) : _cpu(cpu) {}

  InstructionPtr decode() throw(decode_exception);

protected:
  InstructionPtr decodeOneCB(InstructionReader *reader) throw(decode_exception);
  InstructionPtr decodeOne(InstructionReader *reader) throw(decode_exception);

private:
  CPU *_cpu;
};

} // namespace GB