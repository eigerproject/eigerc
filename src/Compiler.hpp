#ifndef _EIGERC_COMPILER_HPP_
#define _EIGERC_COMPILER_HPP_

#include <unordered_map>
#include <vector>

#include "Bytecode.hpp"
#include "Parser.hpp"

namespace EigerC {

enum class BuiltInFunctions { EMITLN = 0 };

class BytecodeCompiler {
   public:
    BytecodeCompiler(std::unique_ptr<ScopeNode> ast) : m_AST(std::move(ast)) {}

    void AddInstruction(Opcode opcode, double operand = 0) {
        m_Code.emplace_back(opcode, operand);
    }

    int GetVariableID(std::string varName) {
        if (m_SymbolTable.contains(varName))
            return m_SymbolTable[varName];
        else {
            int newID = m_SymbolTable.size();
            std::cout << "Declaring variable '" << varName << "' with ID "
                      << newID << std::endl;
            m_SymbolTable[varName] = newID;
            return newID;
        }
    }

    std::vector<Instruction>& DoCodegen() {
        for (const auto& stmt : m_AST->statements) stmt->Codegen(*this);
        return m_Code;
    }

   private:
    std::unordered_map<std::string, int> m_SymbolTable{
        {"emitln",
         (int)BuiltInFunctions::EMITLN}};  // maps variable name with ID
    std::vector<Instruction> m_Code;
    std::unique_ptr<ScopeNode> m_AST;
};

}  // namespace EigerC

#endif  // _EIGERC_COMPILER_HPP_