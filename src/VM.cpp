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

    for (const Instruction& inst : m_Compiler.m_Code) {
        switch (inst.opcode) {
            case Opcode::NO_OP: continue;

            case Opcode::LOAD_VAR:
                m_Stack.push(GetVar(inst.operand, inst.sourceCodeLine));
                break;

            case Opcode::LOAD_IMM: m_Stack.push(EiObject(inst.operand)); break;

            case Opcode::LOAD_STRING:
                m_Stack.push(EiObject(m_Compiler.m_StringTable[inst.operand]));
                break;

            case Opcode::STORE_VAR: {
                EiObject value = PopSafe(inst.sourceCodeLine);
                SetVar(inst.operand, value, inst.sourceCodeLine);
                break;
            }

            case Opcode::DECL_VAR: DeclareVar(inst.operand); break;

            case Opcode::NEW_SCOPE: m_ScopeStack.emplace_back(); break;

            case Opcode::END_SCOPE:
                if (m_ScopeStack.empty()) {
                    throw Error(Error ::Type::RUNTIME_ERROR,
                                "VM Scope Stack underflow",
                                inst.sourceCodeLine);
                }
                m_ScopeStack.pop_back();
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
