#ifndef _EIGERC_COMPILER_HPP_
#define _EIGERC_COMPILER_HPP_

#include <unordered_map>
#include <vector>

#include "Bytecode.hpp"
#include "Parser.hpp"

namespace EigerC {

class BytecodeVM;

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
            m_SymbolTable[varName] = newID;
            return newID;
        }
    }

    int AddString(const std::string& str) {
        int id = m_StringTable.size();
        m_StringTable[id] = str;
        return id;
    }

    void DoCodegen() {
        for (const auto& stmt : m_AST->statements) stmt->Codegen(*this);
    }

   private:
    std::unordered_map<std::string, int> m_SymbolTable{
        {"emitln",
         (int)BuiltInFunctions::EMITLN}};  // maps variable name with ID
    std::unordered_map<int, std::string>
        m_StringTable;  // maps string ID to string value
    std::vector<Instruction> m_Code;
    std::unique_ptr<ScopeNode> m_AST;

    friend class BytecodeVM;
};

}  // namespace EigerC

#endif  // _EIGERC_COMPILER_HPP_