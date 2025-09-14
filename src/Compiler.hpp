#ifndef _EIGERC_COMPILER_HPP_
#define _EIGERC_COMPILER_HPP_

#include <unordered_map>
#include <vector>

#include "Bytecode.hpp"
#include "Context.hpp"
#include "Parser.hpp"

namespace EigerC {

class BytecodeVM;

class BytecodeCompiler {
   public:
    BytecodeCompiler(std::unique_ptr<ScopeNode> ast, CompilerContext& ctx)
        : ast(std::move(ast)), ctx(ctx) {}
    std::vector<Instruction>& GetInstructions() { return code; }

    int GetInstructionPointer() { return static_cast<int>(code.size()); }

    void AddInstruction(Opcode opcode, int line, double operand = 0) {
        code.emplace_back(GetInstructionPointer(), opcode, operand, line);
    }

    void SetInstructionAt(int ip, Opcode opcode, int line, double operand = 0) {
        code[ip] = Instruction(ip, opcode, operand, line);
    }

    void DoCodegen() {
        for (const auto& stmt : ast->statements)
            stmt->Codegen(*this, ctx);
    }

   private:
    std::vector<Instruction> code;
    std::unique_ptr<ScopeNode> ast;
    CompilerContext& ctx;
};

}  // namespace EigerC

#endif  // _EIGERC_COMPILER_HPP_