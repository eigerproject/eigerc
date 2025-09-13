#include "VM.hpp"

#include <iostream>

#include "NumberObject.hpp"

namespace EigerC {

void BytecodeVM::ExecuteBytecode() {
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

    const int instructionCount = static_cast<int>(instructions.size());

    for (int ip = 0; ip < instructionCount; ++ip) {
        const Instruction& inst = instructions[ip];

        switch (inst.opcode) {
            case Opcode::NO_OP: continue;

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

            case Opcode::STORE_VAR: {
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
                std::shared_ptr<EiObject> condition =
                    PopSafe(inst.sourceCodeLine);
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
                                "VM Scope Stack underflow",
                                inst.sourceCodeLine);
                }
                currentScope = currentScope->parent;
                break;

            case Opcode::CALL:
                if (inst.operand ==
                    static_cast<int>(BuiltInFunctions::EMITLN)) {
                    std::shared_ptr<EiObject> val =
                        PopSafe(inst.sourceCodeLine);
                    std::cout << val->AsString() << std::endl;
                } else {
                    throw Error(Error ::Type::NAME_ERROR,
                                "No such built-in function",
                                inst.sourceCodeLine);
                }
                break;

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

            default:
                throw Error(Error ::Type::RUNTIME_ERROR, "Unknown Opcode",
                            inst.sourceCodeLine);
        }
    }
}

}  // namespace EigerC
