#ifndef _EIGERC_VM_HPP_
#define _EIGERC_VM_HPP_

#include <stack>
#include <vector>

#include "Compiler.hpp"
#include "EiObject.hpp"
#include "Error.hpp"

namespace EigerC {

typedef std::unordered_map<int, EiObject> EiScope;

class BytecodeVM {
   public:
    BytecodeVM(BytecodeCompiler &compiler) : m_Compiler(compiler) {
        m_ScopeStack.emplace_back();
    }
    void ExecuteBytecode();

   private:
    void DeclareVar(int id) {
        m_ScopeStack[m_ScopeStack.size() - 1][id] = EiObject();
    }

    void SetVar(int id, EiObject value, int line) {
        ExpectVarExists(id, line);

        for (int i = m_ScopeStack.size() - 1; i >= 0; --i) {
            if (m_ScopeStack[i].contains(id)) {
                m_ScopeStack[i][id] = value;
                break;
            }
        }
    }

    void ExpectVarExists(int id, int line) {
        for (int i = m_ScopeStack.size() - 1; i >= 0; --i) {
            if (m_ScopeStack[i].contains(id)) return;
        }
        for (const auto &[varName, varId] : m_Compiler.m_SymbolTable) {
            if (varId == id) {
                throw Error(Error::Type::NAME_ERROR,
                            std::format("Variable {} not defined in this scope",
                                        varName),
                            line);
            }
        }

        throw Error(Error::Type::NAME_ERROR,
                    std::format("Variable with ID {} not found", id), line);
    }

    EiObject GetVar(int id, int line) {
        ExpectVarExists(id, line);
        for (int i = m_ScopeStack.size() - 1; i >= 0; --i) {
            if (m_ScopeStack[i].contains(id)) { return m_ScopeStack[i][id]; }
        }
    }

   private:
    std::vector<EiScope> m_ScopeStack;
    std::stack<EiObject> m_Stack;

    BytecodeCompiler &m_Compiler;
};

}  // namespace EigerC

#endif  // _EIGERC_VM_HPP_