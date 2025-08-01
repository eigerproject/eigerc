#include "Compiler.hpp"

#include "Error.hpp"

namespace EigerC {

void ScopeNode::Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) {
    compiler.AddInstruction(Opcode::NEW_SCOPE, line);
    for (const auto &stmt : statements) stmt->Codegen(compiler, ctx);
    compiler.AddInstruction(Opcode::END_SCOPE, line);
}

void LetNode::Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) {
    compiler.AddInstruction(Opcode::DECL_VAR, line,
                            ctx.GetVariableID(variableName));
    if (value) {
        value->Codegen(compiler, ctx);
        compiler.AddInstruction(Opcode::STORE_VAR, line,
                                ctx.GetVariableID(variableName));
    }
}

void NumberNode::Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) {
    compiler.AddInstruction(Opcode::LOAD_IMM, line, static_cast<int>(value));
}

void VariableNode::Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) {
    compiler.AddInstruction(Opcode::LOAD_VAR, line, ctx.GetVariableID(name));
}

void StringNode::Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) {
    compiler.AddInstruction(Opcode::LOAD_STRING, line, ctx.AddString(value));
}

void BinaryOpNode::Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) {
    left->Codegen(compiler, ctx);
    right->Codegen(compiler, ctx);

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

void CallNode::Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) {
    for (const auto &arg : arguments) arg->Codegen(compiler, ctx);
    compiler.AddInstruction(Opcode::CALL, line,
                            ctx.GetVariableID(functionName));
}

}  // namespace EigerC