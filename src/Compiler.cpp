#include "Compiler.hpp"

#include "Context.hpp"
#include "Error.hpp"
#include "FunctionObject.hpp"
#include "NumberObject.hpp"
#include "Opcode.hpp"
#include "Parser.hpp"
#include "StringObject.hpp"

namespace EigerC {

void ScopeNode::Codegen(BytecodeCompiler& compiler, CompilerContext& ctx) {
    compiler.AddInstruction(Opcode::NEW_SCOPE, line);
    for (const auto& stmt : statements) stmt->Codegen(compiler, ctx);
    compiler.AddInstruction(Opcode::END_SCOPE, line);
}

void LetNode::Codegen(BytecodeCompiler& compiler, CompilerContext& ctx) {
    compiler.AddInstruction(Opcode::DECL_VAR, line,
                            ctx.GetVariableID(variableName));
    if (value) {
        value->Codegen(compiler, ctx);
        compiler.AddInstruction(Opcode::STORE_VAR, line,
                                ctx.GetVariableID(variableName));
    }
}

void EigerC::IfNode::Codegen(BytecodeCompiler& compiler, CompilerContext& ctx) {
    condition->Codegen(compiler, ctx);

    int ip = compiler.GetInstructionPointer();
    compiler.AddInstruction(Opcode::NO_OP, line);

    ifBlock->Codegen(compiler, ctx);

    int elseIp = compiler.GetInstructionPointer();

    compiler.SetInstructionAt(ip, Opcode::JUMP_IF_FALSE, line,
                              compiler.GetInstructionPointer());

    if (elseBlock) {
        compiler.AddInstruction(Opcode::NO_OP, line);
        elseBlock->Codegen(compiler, ctx);
        compiler.SetInstructionAt(elseIp, Opcode::JUMP, line,
                                  compiler.GetInstructionPointer());
    }
}

void NumberNode::Codegen(BytecodeCompiler& compiler, CompilerContext& ctx) {
    compiler.AddInstruction(Opcode::LOAD_IMM, line, value);
}

void VariableNode::Codegen(BytecodeCompiler& compiler, CompilerContext& ctx) {
    compiler.AddInstruction(Opcode::LOAD_VAR, line, ctx.GetVariableID(name));
}

void StringNode::Codegen(BytecodeCompiler& compiler, CompilerContext& ctx) {
    compiler.AddInstruction(
        Opcode::LOAD_CONST, line,
        ctx.AddConstant(std::make_shared<StringObject>(line, value)));
}

void BinaryOpNode::Codegen(BytecodeCompiler& compiler, CompilerContext& ctx) {
    if (op != TokenType::ASSIGN) left->Codegen(compiler, ctx);
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
        case TokenType::EQ: compiler.AddInstruction(Opcode::EQUAL, line); break;
        case TokenType::NEQ:
            compiler.AddInstruction(Opcode::NEQUAL, line);
            break;
        case TokenType::GT:
            compiler.AddInstruction(Opcode::GREATER, line);
            break;
        case TokenType::GTE:
            compiler.AddInstruction(Opcode::GREATEREQ, line);
            break;
        case TokenType::LT: compiler.AddInstruction(Opcode::LESS, line); break;
        case TokenType::LTE:
            compiler.AddInstruction(Opcode::LESSEQ, line);
            break;
        case TokenType::ASSIGN: {
            Opcode c = Opcode::POP_VAR;

            if (!isAsStatement) c = Opcode::STORE_VAR;

            compiler.AddInstruction(
                c, line,
                ctx.GetVariableID(
                    static_cast<VariableNode*>(left.get())->name));
            break;
        }
        default:
            throw Error(Error::Type::SYNTAX_ERROR, "Unknown binary operation",
                        line);
    }
}

void CallNode::Codegen(BytecodeCompiler& compiler, CompilerContext& ctx) {
    for (const auto& arg : arguments) arg->Codegen(compiler, ctx);
    functionNode->Codegen(compiler, ctx);
    compiler.AddInstruction(Opcode::CALL, line, arguments.size(),
                            !isAsStatement);
}

void EigerC::RetNode::Codegen(BytecodeCompiler& compiler,
                              CompilerContext& ctx) {
    value->Codegen(compiler, ctx);

    Instruction& i = compiler.GetLastInstruction();

    if (i.opcode == Opcode::CALL)
        i.opcode = Opcode::TAIL_CALL;
    else
        compiler.AddInstruction(Opcode::RETURN, line);
}

void EigerC::FunctionNode::Codegen(BytecodeCompiler& compiler,
                                   CompilerContext& ctx) {
    BytecodeCompiler newCompiler;
    body->Codegen(newCompiler, ctx);

    auto& code = newCompiler.GetInstructions();

    if (!isExpression && !code.empty() && code[0].opcode == Opcode::NEW_SCOPE &&
        code[code.size() - 1].opcode == Opcode::END_SCOPE) {
        code.pop_back();
        code.erase(code.begin());

        for (Instruction& inst : code) --inst.address;
    } else
        newCompiler.AddInstruction(Opcode::RETURN, -1);

    auto constv = ctx.AddConstant(std::make_shared<BytecodeFunctionObject>(
        line, ctx, functionName, parameters, code, isExpression));

    compiler.AddInstruction(Opcode::LOAD_FUNC, line, constv);

    if (!this->functionName.empty()) {
        int id = ctx.GetVariableID(functionName);
        compiler.AddInstruction(Opcode::DECL_VAR, line, id);
        compiler.AddInstruction(Opcode::STORE_VAR, line, id);
    }
}

void EigerC::ArrayNode::Codegen(BytecodeCompiler& compiler,
                                CompilerContext& ctx) {
    for (int i = elements.size() - 1; i >= 0; --i)
        elements[i]->Codegen(compiler, ctx);

    compiler.AddInstruction(Opcode::MAKE_ARRAY, line, elements.size());
}

void EigerC::UnaryOpNode::Codegen(BytecodeCompiler& compiler,
                                  CompilerContext& ctx) {
    operand->Codegen(compiler, ctx);

    switch (op) {
        case TokenType::PLUS: break;
        case TokenType::MINUS:
            compiler.AddInstruction(Opcode::NEGATE, line);
            break;

        // `not`
        case TokenType::IDENTIFIER:
            compiler.AddInstruction(Opcode::NOT, line);
            break;

        default: break;
    }
}

void EigerC::IndexNode::Codegen(BytecodeCompiler& compiler,
                                CompilerContext& ctx) {
    indexable->Codegen(compiler, ctx);
    index->Codegen(compiler, ctx);
    compiler.AddInstruction(Opcode::INDEX, line);
}

}  // namespace EigerC
