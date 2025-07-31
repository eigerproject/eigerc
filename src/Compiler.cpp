#include "Compiler.hpp"

#include "Error.hpp"

namespace EigerC {

void ScopeNode::Codegen(BytecodeCompiler &compiler) {
    compiler.AddInstruction(Opcode::NEW_SCOPE, line);
    for (const auto &stmt : statements) stmt->Codegen(compiler);
    compiler.AddInstruction(Opcode::END_SCOPE, line);
}

void LetNode::Codegen(BytecodeCompiler &compiler) {
    compiler.AddInstruction(Opcode::DECL_VAR, line,
                            compiler.GetVariableID(variableName));
    if (value) {
        value->Codegen(compiler);
        compiler.AddInstruction(Opcode::STORE_VAR, line,
                                compiler.GetVariableID(variableName));
    }
}

void NumberNode::Codegen(BytecodeCompiler &compiler) {
    compiler.AddInstruction(Opcode::LOAD_IMM, line, static_cast<int>(value));
}

void VariableNode::Codegen(BytecodeCompiler &compiler) {
    compiler.AddInstruction(Opcode::LOAD_VAR, line,
                            compiler.GetVariableID(name));
}

void StringNode::Codegen(BytecodeCompiler &compiler) {
    compiler.AddInstruction(Opcode::LOAD_STRING, line,
                            compiler.AddString(value));
}

void BinaryOpNode::Codegen(BytecodeCompiler &compiler) {
    left->Codegen(compiler);
    right->Codegen(compiler);

    switch (op) {
        case TokenType::PLUS: compiler.AddInstruction(Opcode::ADD, line); break;
        case TokenType::MINUS:
            compiler.AddInstruction(Opcode::SUBTRACT, line);
            break;
        case TokenType::MULTIPLY:
            compiler.AddInstruction(Opcode::MULTIPLY, line);
            break;
        case TokenType::DIVIDE:
            compiler.AddInstruction(Opcode::DIVIDE, line);
            break;
        default:
            throw Error(Error::Type::SYNTAX_ERROR, "Unknown binary operation",
                        line);
    }
}

void CallNode::Codegen(BytecodeCompiler &compiler) {
    for (const auto &arg : arguments) arg->Codegen(compiler);
    compiler.AddInstruction(Opcode::CALL, line,
                            compiler.GetVariableID(functionName));
}

}  // namespace EigerC