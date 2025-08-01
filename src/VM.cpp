#include "VM.hpp"

#include <iostream>
#include <stdexcept>

namespace EigerC {

void BytecodeVM::ExecuteBytecode() {
    auto PopSafe = [&](int line) -> EiObject {
        if (m_Stack.empty()) {
            throw Error(Error ::Type::RUNTIME_ERROR, "VM Stack underflow",
                        line);
        }
        EiObject val = m_Stack.top();
        m_Stack.pop();
        return val;
    };

    auto BinaryOp = [&](const Instruction& inst, auto operation) {
        EiObject rhs = PopSafe(inst.sourceCodeLine);
        EiObject lhs = PopSafe(inst.sourceCodeLine);
        m_Stack.push(operation(lhs, rhs));
    };

    const auto& instructions = m_Compiler.GetInstructions();
    const int instructionCount = instructions.size();

    for (int ip = 0; ip < instructionCount; ++ip) {
        const Instruction& inst = instructions[ip];

        switch (inst.opcode) {
            case Opcode::NO_OP: continue;

            case Opcode::LOAD_VAR:
                m_Stack.push(
                    m_CurrentScope->GetVar(inst.operand, inst.sourceCodeLine));
                break;

            case Opcode::LOAD_IMM: m_Stack.push(EiObject(inst.operand)); break;

            case Opcode::LOAD_CONST:
                m_Stack.push(m_Context.constantsTable[inst.operand]);
                break;

            case Opcode::STORE_VAR: {
                EiObject value = PopSafe(inst.sourceCodeLine);
                m_CurrentScope->SetVar(inst.operand, value,
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
                m_CurrentScope->DeclareVar(inst.operand);
                break;

            case Opcode::NEW_SCOPE:
                m_CurrentScope =
                    std::make_shared<Scope>(m_Context, m_CurrentScope);
                break;

            case Opcode::END_SCOPE:
                if (!m_CurrentScope->parent) {
                    throw Error(Error ::Type::RUNTIME_ERROR,
                                "VM Scope Stack underflow",
                                inst.sourceCodeLine);
                }
                m_CurrentScope = m_CurrentScope->parent;
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
                         [](EiObject& a, EiObject& b) { return a.Add(b); });
                break;

            case Opcode::SUBTRACT:
                BinaryOp(inst, [](EiObject& a, EiObject& b) {
                    return a.Subtract(b);
                });
                break;

            case Opcode::MULTIPLY:
                BinaryOp(inst, [](EiObject& a, EiObject& b) {
                    return a.Multiply(b);
                });
                break;

            case Opcode::DIVIDE:
                BinaryOp(inst,
                         [](EiObject& a, EiObject& b) { return a.Divide(b); });
                break;

            default:
                throw Error(Error ::Type::RUNTIME_ERROR, "Unknown Opcode",
                            inst.sourceCodeLine);
        }
    }
}

}  // namespace EigerC
