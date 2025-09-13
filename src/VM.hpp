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
        : instructions(compiler.GetInstructions()), ctx(ctx) {
        currentScope = std::make_shared<Scope>(ctx);
    }

    BytecodeVM(const std::vector<Instruction> &instructions,
               CompilerContext &ctx, std::shared_ptr<Scope> scope)
        : instructions(instructions), ctx(ctx) {
        currentScope = scope;
    }

    void ExecuteBytecode();

   private:
    std::shared_ptr<Scope> currentScope;
    std::stack<std::shared_ptr<EiObject>> stack;

    const std::vector<Instruction> &instructions;
    CompilerContext &ctx;
};

}  // namespace EigerC

#endif  // _EIGERC_VM_HPP_