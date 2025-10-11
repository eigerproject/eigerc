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

struct CallFrame {
    const std::vector<Instruction> &code;
    size_t ip;
    std::shared_ptr<Scope> scope;
    size_t stackBase;
    bool pushReturnValue;
};

class BytecodeVM {
   public:
    BytecodeVM(const std::vector<Instruction> &instructions,
               CompilerContext &ctx, std::shared_ptr<Scope> scope)
        : ctx(ctx) {
        NewCallFrame(instructions, scope);
    }

    void ExecuteBytecode();

    std::shared_ptr<EiObject> Peek(int line = -1);
    bool CanPeek() { return !stack.empty(); }

    void NewCallFrame(const std::vector<Instruction> &code,
                      const std::shared_ptr<Scope> &scope,
                      bool pushRetVal = false) {
        callStack.push(CallFrame{code, 0, scope, stack.size(), pushRetVal});
    }

    void PopCallFrame() {
        size_t newSize = callStack.top().stackBase;
        while (stack.size() > newSize) stack.pop();
        callStack.pop();
    }

   private:
    void ExecuteNextInstruction();

    std::stack<std::shared_ptr<EiObject>> stack;
    std::stack<CallFrame> callStack;
    CompilerContext &ctx;
};

}  // namespace EigerC

#endif  // _EIGERC_VM_HPP_