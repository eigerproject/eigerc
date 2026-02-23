#include "FunctionObject.hpp"

namespace EigerC {

BytecodeFunctionObject::BytecodeFunctionObject(
    int line, CompilerContext& ctx, std::string name,
    std::vector<std::string> argNames, std::vector<Instruction> code,
    bool isInline)
    : ctx(ctx), code(code), isInline(isInline), FunctionObject(name, argNames) {
    this->line = line;
    if (ctx.cmdOpts.verbose)
        for (const Instruction& c : code) c.PrettyPrint();
}

void BytecodeFunctionObject::StartExecute(
    const std::vector<std::shared_ptr<EiObject>>& values, BytecodeVM* vm,
    bool pushRetVal) {
    auto fScope = std::make_shared<Scope>(ctx, closure);

    for (size_t i = 0; i < argNames.size(); ++i) {
        int id = ctx.GetVariableID(argNames[i]);
        fScope->DeclareVar(id);
        fScope->SetVar(id, values[i], line);
    }

    vm->NewCallFrame(code, fScope, pushRetVal);
}

}  // namespace EigerC
