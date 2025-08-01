#ifndef _EIGERC_VM_HPP_
#define _EIGERC_VM_HPP_

#include <stack>
#include <vector>

#include "Compiler.hpp"
#include "Context.hpp"
#include "EiObject.hpp"
#include "Error.hpp"
#include "Scope.hpp"

namespace EigerC {

class BytecodeVM {
   public:
    BytecodeVM(BytecodeCompiler &compiler, CompilerContext &ctx)
        : m_Compiler(compiler), m_Context(ctx) {
        m_CurrentScope = std::make_shared<Scope>(ctx);
    }

    void ExecuteBytecode();

   private:
    std::shared_ptr<Scope> m_CurrentScope;
    std::stack<EiObject> m_Stack;

    BytecodeCompiler &m_Compiler;
    CompilerContext &m_Context;
};

}  // namespace EigerC

#endif  // _EIGERC_VM_HPP_