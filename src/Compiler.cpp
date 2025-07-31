#include "Compiler.hpp"

#include "Error.hpp"

namespace EigerC {

void ScopeNode::Codegen(BytecodeCompiler &compiler) {
    compiler.AddInstruction(Opcode::NEW_SCOPE);
    for (const auto &stmt : statements) stmt->Codegen(compiler);
    compiler.AddInstruction(Opcode::END_SCOPE);
}

void LetNode::Codegen(BytecodeCompiler &compiler) {
    compiler.AddInstruction(Opcode::DECL_VAR,
                            compiler.GetVariableID(variableName));
    if (value) {
        value->Codegen(compiler);
        compiler.AddInstruction(Opcode::STORE_VAR,
                                compiler.GetVariableID(variableName));
    }
}

void NumberNode::Codegen(BytecodeCompiler &compiler) {
    compiler.AddInstruction(Opcode::LOAD_IMM, static_cast<int>(value));
}

void VariableNode::Codegen(BytecodeCompiler &compiler) {
    compiler.AddInstruction(Opcode::LOAD_VAR, compiler.GetVariableID(name));
}

void BinaryOpNode::Codegen(BytecodeCompiler &compiler) {
    left->Codegen(compiler);
    right->Codegen(compiler);

    switch (op) {
        case TokenType::PLUS: compiler.AddInstruction(Opcode::ADD); break;
        case TokenType::MINUS: compiler.AddInstruction(Opcode::SUBTRACT); break;
        case TokenType::MULTIPLY:
            compiler.AddInstruction(Opcode::MULTIPLY);
            break;
        case TokenType::DIVIDE: compiler.AddInstruction(Opcode::DIVIDE); break;
        default:
            throw Error(Error::Type::SYNTAX_ERROR, "Unknown binary operation",
                        opLine);
    }
}

void CallNode::Codegen(BytecodeCompiler &compiler) {
    for (const auto &arg : arguments) arg->Codegen(compiler);
    compiler.AddInstruction(Opcode::CALL, compiler.GetVariableID(functionName));
}

}  // namespace EigerC