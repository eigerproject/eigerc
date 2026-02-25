#include "VM.hpp"

#include "ArrayObject.hpp"
#include "FunctionObject.hpp"
#include "NumberObject.hpp"

namespace EigerC {

void BytecodeVM::ExecuteNextInstruction() {
    auto PopSafe = [&](const int line) -> std::shared_ptr<EiObject> {
        if (stack.empty()) {
            throw Error(Error ::Type::RUNTIME_ERROR, "VM Stack underflow",
                        line);
        }
        std::shared_ptr<EiObject> val = stack.top();
        stack.pop();
        return val;
    };

    auto BinaryOp = [&](const Instruction& inst, auto operation) {
        std::shared_ptr<EiObject> rhs = PopSafe(inst.sourceCodeLine);
        std::shared_ptr<EiObject> lhs = PopSafe(inst.sourceCodeLine);
        stack.push(operation(lhs, rhs));
    };

    CallFrame& frame = callStack.top();
    auto& currentScope = frame.scope;
    size_t& ip = frame.ip;
    bool pushReturnValue = frame.pushReturnValue;

    if (ip >= frame.code.size()) {
        PopCallFrame();
        if (pushReturnValue) stack.push(std::make_shared<NixObject>());
        return;
    }

    const Instruction& inst = frame.code[frame.ip];

    switch (inst.opcode) {
        case Opcode::NO_OP: break;

        case Opcode::LOAD_VAR:
            stack.push(currentScope->GetVar(static_cast<int>(inst.operand),
                                            inst.sourceCodeLine));
            break;

        case Opcode::LOAD_IMM:
            stack.push(std::make_shared<NumberObject>(inst.sourceCodeLine,
                                                      inst.operand));
            break;

        case Opcode::LOAD_CONST:
            stack.push(ctx.constantsTable[static_cast<int>(inst.operand)]);
            break;

        case Opcode::LOAD_FUNC: {
            const auto& constant =
                ctx.constantsTable[static_cast<int>(inst.operand)];

            if (constant->type != DType::FUNCTION) {
                throw Error(Error ::Type::TYPE_ERROR,
                            std::format("{} is not callable",
                                        ctx.GetVarName(inst.operand)),
                            inst.sourceCodeLine);
            }

            std::shared_ptr<BytecodeFunctionObject> fn =
                std::dynamic_pointer_cast<BytecodeFunctionObject>(constant);

            fn->SetClosure(currentScope);
            stack.push(fn);

            break;
        }

        case Opcode::MAKE_ARRAY: {
            std::vector<std::shared_ptr<EiObject>> elements;
            for (int i = 0; i < inst.operand; ++i)
                elements.push_back(PopSafe(inst.sourceCodeLine));

            stack.push(
                std::make_shared<ArrayObject>(inst.sourceCodeLine, elements));

            break;
        }

        case Opcode::STORE_VAR: {
            std::shared_ptr<EiObject> value = Peek(inst.sourceCodeLine);
            currentScope->SetVar(static_cast<int>(inst.operand), value,
                                 inst.sourceCodeLine);
            break;
        }

        case Opcode::POP_VAR: {
            std::shared_ptr<EiObject> value = PopSafe(inst.sourceCodeLine);
            currentScope->SetVar(static_cast<int>(inst.operand), value,
                                 inst.sourceCodeLine);
            break;
        }

        case Opcode::JUMP: {
            ip = static_cast<int>(inst.operand) - 1;
            break;
        }

        case Opcode::JUMP_IF_FALSE: {
            std::shared_ptr<EiObject> condition = PopSafe(inst.sourceCodeLine);
            if ((bool)*condition == false)
                ip = static_cast<int>(inst.operand) - 1;
            break;
        }

        case Opcode::DECL_VAR:
            currentScope->DeclareVar(static_cast<int>(inst.operand));
            break;

        case Opcode::NEW_SCOPE:
            currentScope = std::make_shared<Scope>(ctx, currentScope);
            break;

        case Opcode::END_SCOPE:
            if (!currentScope->parent) {
                throw Error(Error ::Type::RUNTIME_ERROR,
                            "VM Scope Stack underflow", inst.sourceCodeLine);
            }
            currentScope = currentScope->parent;
            break;

        case Opcode::CALL: {
            std::shared_ptr<EiObject> fnObj = PopSafe(inst.sourceCodeLine);

            if (fnObj->type != DType::FUNCTION) {
                throw Error(
                    Error ::Type::TYPE_ERROR,
                    std::format("Object {} is not callable", fnObj->AsString()),
                    inst.sourceCodeLine);
            }

            std::shared_ptr<FunctionObject> fn =
                std::dynamic_pointer_cast<FunctionObject>(fnObj);

            // get args from stack
            int argCount = inst.operand;
            std::vector<std::shared_ptr<EiObject>> args(argCount);

            for (int i = argCount - 1; i >= 0; --i)
                args[i] = PopSafe(inst.sourceCodeLine);

            if (auto builtinFn =
                    std::dynamic_pointer_cast<BuiltinFunctionObject>(fn)) {
                auto val = builtinFn->Execute(args);
                if (inst.flag) stack.push(val);
            } else if (auto bcFn =
                           std::dynamic_pointer_cast<BytecodeFunctionObject>(
                               fn)) {
                bool doTailCallOp = ip >= frame.code.size() - 1;
                if (doTailCallOp) PopCallFrame();
                bcFn->StartExecute(args, this, inst.flag);
                if (doTailCallOp) return;
            }

            break;
        }

        case Opcode::RETURN: {
            auto val = PopSafe(inst.sourceCodeLine);

            PopCallFrame();
            if (pushReturnValue) stack.push(val);
            return;
        }

        case Opcode::ADD:
            BinaryOp(inst, [](const std::shared_ptr<EiObject>& a,
                              const std::shared_ptr<EiObject>& b) {
                return *a + *b;
            });
            break;

        case Opcode::SUBTRACT:
            BinaryOp(inst, [](const std::shared_ptr<EiObject>& a,
                              const std::shared_ptr<EiObject>& b) {
                return *a - *b;
            });
            break;

        case Opcode::MULTIPLY:
            BinaryOp(inst, [](const std::shared_ptr<EiObject>& a,
                              const std::shared_ptr<EiObject>& b) {
                return *a * *b;
            });
            break;

        case Opcode::DIVIDE:
            BinaryOp(inst, [](const std::shared_ptr<EiObject>& a,
                              const std::shared_ptr<EiObject>& b) {
                return *a / *b;
            });
            break;

        case Opcode::EQUAL:
            BinaryOp(inst, [](const std::shared_ptr<EiObject>& a,
                              const std::shared_ptr<EiObject>& b) {
                return *a == *b;
            });
            break;

        case Opcode::NEQUAL:
            BinaryOp(inst, [](const std::shared_ptr<EiObject>& a,
                              const std::shared_ptr<EiObject>& b) {
                return *a != *b;
            });
            break;

        case Opcode::GREATER:
            BinaryOp(inst, [](const std::shared_ptr<EiObject>& a,
                              const std::shared_ptr<EiObject>& b) {
                return *a > *b;
            });
            break;

        case Opcode::GREATEREQ:
            BinaryOp(inst, [](const std::shared_ptr<EiObject>& a,
                              const std::shared_ptr<EiObject>& b) {
                return *a >= *b;
            });
            break;

        case Opcode::LESS:
            BinaryOp(inst, [](const std::shared_ptr<EiObject>& a,
                              const std::shared_ptr<EiObject>& b) {
                return *a < *b;
            });
            break;

        case Opcode::LESSEQ:
            BinaryOp(inst, [](const std::shared_ptr<EiObject>& a,
                              const std::shared_ptr<EiObject>& b) {
                return *a <= *b;
            });
            break;

        default:
            throw Error(Error ::Type::RUNTIME_ERROR, "Unknown Opcode",
                        inst.sourceCodeLine);
    }

    ++ip;
}

void BytecodeVM::ExecuteBytecode() {
    while (!callStack.empty()) ExecuteNextInstruction();
}

std::shared_ptr<EiObject> BytecodeVM::Peek(int line) {
    if (stack.empty()) {
        throw Error(Error ::Type::RUNTIME_ERROR, "VM Stack underflow", line);
    }
    return stack.top();
}

}  // namespace EigerC
