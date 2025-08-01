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
        : m_AST(std::move(ast)), m_Context(ctx) {}
    std::vector<Instruction>& GetInstructions() { return m_Code; }

    void AddInstruction(Opcode opcode, int line, double operand = 0) {
        m_Code.emplace_back(opcode, operand, line);
    }

    void DoCodegen() {
        for (const auto& stmt : m_AST->statements)
            stmt->Codegen(*this, m_Context);
    }

   private:
    std::vector<Instruction> m_Code;
    std::unique_ptr<ScopeNode> m_AST;
    CompilerContext& m_Context;
};

}  // namespace EigerC

#endif  // _EIGERC_COMPILER_HPP_