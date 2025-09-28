#ifndef _EIGERC_FUNCTIONOBJECT_HPP_
#define _EIGERC_FUNCTIONOBJECT_HPP_

#include <memory>
#include <string>
#include <vector>

#include "EiObject.hpp"
#include "Instruction.hpp"
#include "Scope.hpp"
#include "VM.hpp"

namespace EigerC {

class FunctionObject : public EiObject {
   public:
    FunctionObject() = default;
    FunctionObject(std::string name, std::vector<std::string> argNames)
        : name(name), argNames(argNames) {
        type = DType::FUNCTION;
    }

    virtual ~FunctionObject() = default;

    virtual std::shared_ptr<EiObject> Execute(
        const std::vector<std::shared_ptr<EiObject>>& values) = 0;

    std::string name;
    std::vector<std::string> argNames;
};

class BuiltinFunctionObject : public FunctionObject {
   public:
    BuiltinFunctionObject(std::string name, std::vector<std::string> argNames)
        : FunctionObject(name, argNames) {
        type = DType::FUNCTION;
    }

    std::string AsString() const override {
        return std::format("<built-in function {}>", name);
    }
};

class Emitln : public BuiltinFunctionObject {
   public:
    Emitln() : BuiltinFunctionObject("emitln", {"value"}) {}
    std::shared_ptr<EiObject> Execute(
        const std::vector<std::shared_ptr<EiObject>>& values) override {
        std::cout << values[0]->AsString() << std::endl;
        return std::make_shared<NixObject>();
    }
};

class BytecodeFunctionObject : public FunctionObject {
   public:
    BytecodeFunctionObject(int line, CompilerContext& ctx, std::string name,
                           std::vector<std::string> argNames,
                           std::vector<Instruction> code, bool isInline)
        : ctx(ctx),
          code(code),
          isInline(isInline),
          FunctionObject(name, argNames) {
        this->line = line;
    }

    std::shared_ptr<EiObject> Execute(
        const std::vector<std::shared_ptr<EiObject>>& values) override {
        auto fScope = std::make_shared<Scope>(ctx, closure);
        BytecodeVM vm(code, ctx, fScope);

        for (size_t i = 0; i < argNames.size(); ++i) {
            int id = ctx.GetVariableID(argNames[i]);
            fScope->DeclareVar(id);
            fScope->SetVar(id, values[i], line);
        }

        auto result = vm.ExecuteBytecode();
        if (isInline) return vm.Peek();
        return result;
    }

    std::string AsString() const override {
        std::string_view funcType = isInline ? "inline function" : "function";
        if (name.empty())
            return std::format("<{}>", funcType);
        else
            return std::format("<{} {}>", funcType, name);
    }

    void SetClosure(std::shared_ptr<Scope> closure) { this->closure = closure; }
    const std::shared_ptr<Scope>& GetClosure() const { return closure; }

   private:
    std::shared_ptr<Scope> closure;
    int isInline = false;
    std::vector<Instruction> code;
    CompilerContext& ctx;
};

}  // namespace EigerC

#endif  // _EIGERC_FUNCTIONOBJECT_HPP_
