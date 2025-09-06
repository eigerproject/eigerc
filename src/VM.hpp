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
        : compiler(compiler), ctx(ctx) {
        currentScope = std::make_shared<Scope>(ctx);
    }

    void ExecuteBytecode();

   private:
    std::shared_ptr<Scope> currentScope;
    std::stack<EiObject> stack;

    BytecodeCompiler &compiler;
    CompilerContext &ctx;
};

}  // namespace EigerC

#endif  // _EIGERC_VM_HPP_