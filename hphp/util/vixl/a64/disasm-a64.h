/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2013 Facebook, Inc. (http://www.facebook.com)     |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/
// Copyright 2013, ARM Limited
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of ARM Limited nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef VIXL_A64_DISASM_A64_H
#define VIXL_A64_DISASM_A64_H

#include "hphp/util/vixl/globals.h"
#include "hphp/util/vixl/utils.h"
#include "instructions-a64.h"
#include "decoder-a64.h"

namespace vixl {

class Disassembler: public DecoderVisitor {
 public:
  Disassembler();
  Disassembler(char* text_buffer, int buffer_size);
  virtual ~Disassembler();
  char* GetOutput();

  // Declare all Visitor functions.
  #define DECLARE(A)  void Visit##A(Instruction* instr);
  VISITOR_LIST(DECLARE)
  #undef DECLARE

 protected:
  virtual void ProcessOutput(Instruction* instr);

 private:
  void Format(Instruction* instr, const char* mnemonic, const char* format);
  void Substitute(Instruction* instr, const char* string);
  int SubstituteField(Instruction* instr, const char* format);
  int SubstituteRegisterField(Instruction* instr, const char* format);
  int SubstituteImmediateField(Instruction* instr, const char* format);
  int SubstituteLiteralField(Instruction* instr, const char* format);
  int SubstituteBitfieldImmediateField(Instruction* instr, const char* format);
  int SubstituteShiftField(Instruction* instr, const char* format);
  int SubstituteExtendField(Instruction* instr, const char* format);
  int SubstituteConditionField(Instruction* instr, const char* format);
  int SubstitutePCRelAddressField(Instruction* instr, const char* format);
  int SubstituteBranchTargetField(Instruction* instr, const char* format);
  int SubstituteLSRegOffsetField(Instruction* instr, const char* format);
  int SubstitutePrefetchField(Instruction* instr, const char* format);

  inline bool RdIsZROrSP(Instruction* instr) const {
    return (instr->Rd() == kZeroRegCode);
  }

  inline bool RnIsZROrSP(Instruction* instr) const {
    return (instr->Rn() == kZeroRegCode);
  }

  inline bool RmIsZROrSP(Instruction* instr) const {
    return (instr->Rm() == kZeroRegCode);
  }

  inline bool RaIsZROrSP(Instruction* instr) const {
    return (instr->Ra() == kZeroRegCode);
  }

  bool IsMovzMovnImm(unsigned reg_size, uint64_t value);

  void ResetOutput();
  void AppendToOutput(const char* string, ...);

  char* buffer_;
  uint32_t buffer_pos_;
  uint32_t buffer_size_;
  bool own_buffer_;
};


class PrintDisassembler: public Disassembler {
 public:
  explicit PrintDisassembler(FILE* stream) : stream_(stream) { }
  ~PrintDisassembler() { }

 protected:
  virtual void ProcessOutput(Instruction* instr);

 private:
  FILE *stream_;
};
}  // namespace vixl

#endif  // VIXL_A64_DISASM_A64_H
