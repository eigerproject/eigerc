#include "VM.hpp"

#include <iostream>

namespace EigerC {

void BytecodeVM::ExecuteBytecode() {
    auto PopSafe = [&](const int line) -> EiObject {
        if (stack.empty()) {
            throw Error(Error ::Type::RUNTIME_ERROR, "VM Stack underflow",
                        line);
        }
        EiObject val = stack.top();
        stack.pop();
        return val;
    };

    auto BinaryOp = [&](const Instruction& inst, auto operation) {
        EiObject rhs = PopSafe(inst.sourceCodeLine);
        EiObject lhs = PopSafe(inst.sourceCodeLine);
        stack.push(operation(lhs, rhs));
    };

    const auto& instructions = compiler.GetInstructions();
    const int instructionCount = static_cast<int>(instructions.size());

    for (int ip = 0; ip < instructionCount; ++ip) {
        const Instruction& inst = instructions[ip];

        switch (inst.opcode) {
            case Opcode::NO_OP: continue;

            case Opcode::LOAD_VAR:
                stack.push(
                    currentScope->GetVar(static_cast<int>(inst.operand), inst.sourceCodeLine));
                break;

            case Opcode::LOAD_IMM: stack.emplace(inst.operand); break;

            case Opcode::LOAD_CONST:
                stack.push(ctx.constantsTable[static_cast<int>(inst.operand)]);
                break;

            case Opcode::STORE_VAR: {
                EiObject value = PopSafe(inst.sourceCodeLine);
                currentScope->SetVar(static_cast<int>(inst.operand), value,
                                       inst.sourceCodeLine);
                break;
            }

            case Opcode::JUMP: {
                ip = static_cast<int>(inst.operand) - 1;
                break;
            }

            case Opcode::JUMP_IF_FALSE: {
                EiObject condition = PopSafe(inst.sourceCodeLine);
                if (!condition.IsTruthy())
                    ip = static_cast<int>(inst.operand) - 1;

                break;
            }

            case Opcode::DECL_VAR:
                currentScope->DeclareVar(static_cast<int>(inst.operand));
                break;

            case Opcode::NEW_SCOPE:
                currentScope =
                    std::make_shared<Scope>(ctx, currentScope);
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
                    EiObject val = PopSafe(inst.sourceCodeLine);
                    std::cout << val.AsString() << std::endl;
                } else {
                    throw Error(Error ::Type::NAME_ERROR,
                                "No such built-in function",
                                inst.sourceCodeLine);
                }
                break;

            case Opcode::ADD:
                BinaryOp(inst,
                         [](const EiObject& a, const EiObject& b) { return a.Add(b); });
                break;

            case Opcode::SUBTRACT:
                BinaryOp(inst, [](const EiObject& a, const EiObject& b) {
                    return a.Subtract(b);
                });
                break;

            case Opcode::MULTIPLY:
                BinaryOp(inst, [](const EiObject& a, const EiObject& b) {
                    return a.Multiply(b);
                });
                break;

            case Opcode::DIVIDE:
                BinaryOp(inst,
                         [](const EiObject& a, const EiObject& b) { return a.Divide(b); });
                break;

            default:
                throw Error(Error ::Type::RUNTIME_ERROR, "Unknown Opcode",
                            inst.sourceCodeLine);
        }
    }
}

}  // namespace EigerC
