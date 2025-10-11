#ifndef _EIGERC_COMPILER_HPP_
#define _EIGERC_COMPILER_HPP_

#include <vector>

#include "Instruction.hpp"
#include "Parser.hpp"

namespace EigerC {

class BytecodeVM;

class BytecodeCompiler {
   public:
    BytecodeCompiler() {}

    std::vector<Instruction>& GetInstructions() { return code; }

    int GetInstructionPointer() { return static_cast<int>(code.size()); }

    void AddInstruction(Opcode opcode, int line, double operand = 0,
                        bool flag = false) {
        code.emplace_back(GetInstructionPointer(), opcode, operand, flag, line);
    }

    void SetInstructionAt(int ip, Opcode opcode, int line, double operand = 0,
                          bool flag = false) {
        code[ip] = Instruction(ip, opcode, operand, flag, line);
    }

   private:
    std::vector<Instruction> code;
};

}  // namespace EigerC

#endif  // _EIGERC_COMPILER_HPP_
