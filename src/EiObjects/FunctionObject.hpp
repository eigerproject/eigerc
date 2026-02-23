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

    std::string name;
    std::vector<std::string> argNames;

    // to add later
    // if true, last variable name in argNames is the variadic collector
    // bool isVariadic = false;
};

class BuiltinFunctionObject : public FunctionObject {
   public:
    BuiltinFunctionObject(std::string name, std::vector<std::string> argNames)
        : FunctionObject(name, argNames) {
        type = DType::FUNCTION;
    }

    virtual std::shared_ptr<EiObject> Execute(
        const std::vector<std::shared_ptr<EiObject>>& values) = 0;

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
                           std::vector<Instruction> code, bool isInline);

    void StartExecute(const std::vector<std::shared_ptr<EiObject>>& values,
                      BytecodeVM* vm, bool pushRetVal);

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
