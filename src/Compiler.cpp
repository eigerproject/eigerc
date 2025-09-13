#include "Compiler.hpp"

#include "Error.hpp"
#include "FunctionObject.hpp"
#include "NumberObject.hpp"
#include "StringObject.hpp"

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

void EigerC::IfNode::Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) {
    condition->Codegen(compiler, ctx);

    int ip = compiler.GetInstructionPointer();
    compiler.AddInstruction(Opcode::NO_OP, line);

    ifBlock->Codegen(compiler, ctx);

    int elseIp = compiler.GetInstructionPointer();
    if (elseBlock) compiler.AddInstruction(Opcode::NO_OP, line);

    compiler.SetInstructionAt(ip, Opcode::JUMP_IF_FALSE, line,
                              compiler.GetInstructionPointer());

    if (elseBlock) {
        elseBlock->Codegen(compiler, ctx);
        compiler.SetInstructionAt(elseIp, Opcode::JUMP, line,
                                  compiler.GetInstructionPointer());
    }
}

void NumberNode::Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) {
    compiler.AddInstruction(Opcode::LOAD_IMM, line, static_cast<int>(value));
}

void VariableNode::Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) {
    compiler.AddInstruction(Opcode::LOAD_VAR, line, ctx.GetVariableID(name));
}

void StringNode::Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) {
    compiler.AddInstruction(
        Opcode::LOAD_CONST, line,
        ctx.AddConstant(std::make_shared<StringObject>(line, value)));
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

void EigerC::FunctionNode::Codegen(BytecodeCompiler &compiler,
                                   CompilerContext &ctx) {
    std::unique_ptr<ScopeNode> root = std::make_unique<ScopeNode>(line);
    root->statements.push_back(std::move(body));

    BytecodeCompiler newCompiler(std::move(root), ctx);
    newCompiler.DoCodegen();

    auto constv = ctx.AddConstant(std::make_shared<BytecodeFunctionObject>(
        line, ctx, functionName, parameters, newCompiler.GetInstructions()));

    compiler.AddInstruction(Opcode::LOAD_CONST, line, constv);

    if (!this->functionName.empty()) {
        int id = ctx.GetVariableID(functionName);
        compiler.AddInstruction(Opcode::DECL_VAR, line, id);
        compiler.AddInstruction(Opcode::STORE_VAR, line, id);
    }
}

}  // namespace EigerC