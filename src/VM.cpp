#include "VM.hpp"

#include <iostream>

namespace EigerC {

void BytecodeVM::ExecuteBytecode() {
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

            case Opcode::STORE_VAR:
                SetVar(inst.operand, m_Stack.top(), inst.sourceCodeLine);
                m_Stack.pop();
                break;

            case Opcode::DECL_VAR: DeclareVar(inst.operand); break;

            case Opcode::NEW_SCOPE: m_ScopeStack.emplace_back(); break;
            case Opcode::END_SCOPE: m_ScopeStack.pop_back(); break;

            case Opcode::CALL:
                if (inst.operand == (int)BuiltInFunctions::EMITLN) {
                    std::cout << m_Stack.top().AsString() << std::endl;
                    m_Stack.pop();
                }
                break;

            case Opcode::ADD: {
                EiObject a2 = m_Stack.top();
                m_Stack.pop();
                EiObject a1 = m_Stack.top();
                m_Stack.pop();

                m_Stack.push(a1.Add(a2));
                break;
            }

            case Opcode::SUBTRACT: {
                EiObject a2 = m_Stack.top();
                m_Stack.pop();
                EiObject a1 = m_Stack.top();
                m_Stack.pop();

                m_Stack.push(a1.Subtract(a2));
                break;
            }

            case Opcode::MULTIPLY: {
                EiObject a2 = m_Stack.top();
                m_Stack.pop();
                EiObject a1 = m_Stack.top();
                m_Stack.pop();

                m_Stack.push(a1.Multiply(a2));
                break;
            }

            case Opcode::DIVIDE: {
                EiObject a2 = m_Stack.top();
                m_Stack.pop();
                EiObject a1 = m_Stack.top();
                m_Stack.pop();

                m_Stack.push(a1.Divide(a2));
                break;
            }
        }
    }
}

}  // namespace EigerC