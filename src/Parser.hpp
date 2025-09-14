#ifndef _EIGERC_PARSER_HPP_
#define _EIGERC_PARSER_HPP_

#include <format>
#include <iostream>
#include <memory>
#include <vector>

#include "Context.hpp"
#include "Lexer.hpp"
#include "Util.hpp"

namespace EigerC {

class BytecodeCompiler;

struct ASTNode {
    int line;

    ASTNode(int line) : line(line) {}

    virtual ~ASTNode() = default;

    virtual void PrettyPrint(int indent = 0) = 0;

    virtual void Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) = 0;
};

struct ScopeNode : public ASTNode {
    std::vector<std::unique_ptr<ASTNode>> statements{};

    ScopeNode(int line) : ASTNode(line) {}

    void PrettyPrint(int indent = 0) override {
        std::string indentStr(indent, '\t');
        std::cout << indentStr << "SCOPE" << std::endl;
        for (const auto &stmt : statements) stmt->PrettyPrint(indent + 1);
    }

    void Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) override;
};

struct IfNode : public ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> ifBlock;
    std::unique_ptr<ASTNode> elseBlock;

    IfNode(std::unique_ptr<ASTNode> c, std::unique_ptr<ASTNode> ib, int line,
           std::unique_ptr<ASTNode> eb = nullptr)
        : condition(std::move(c)),
          ifBlock(std::move(ib)),
          elseBlock(std::move(eb)),
          ASTNode(line) {}

    void PrettyPrint(int indent = 0) override {
        std::string indentStr(indent, '\t');
        std::cout << indentStr << "IF" << std::endl;
        condition->PrettyPrint(indent + 1);
        ifBlock->PrettyPrint(indent + 1);
        if (elseBlock) elseBlock->PrettyPrint(indent + 1);
    }

    void Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) override;
};

struct FunctionNode : public ASTNode {
    std::string functionName;
    std::vector<std::string> parameters;
    std::unique_ptr<ASTNode> body;

    FunctionNode(std::string functionName, std::vector<std::string> parameters,
                 std::unique_ptr<ASTNode> body, int line)
        : functionName(std::move(functionName)),
          parameters(std::move(parameters)),
          body(std::move(body)),
          ASTNode(line) {}

    void PrettyPrint(int indent = 0) override {
        std::string indentStr(indent, '\t');
        std::cout << indentStr << "FUNCTION " << functionName << "( ";
        for (const auto &param : parameters)
            std::cout << param << " ";
        std::cout << ")" << std::endl;
        if (body) body->PrettyPrint(indent + 1);
    }

    void Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) override;
};

struct LetNode : public ASTNode {
    std::string variableName;
    std::unique_ptr<ASTNode> value;
    LetNode(std::string variableName, std::unique_ptr<ASTNode> value, int line)
        : variableName(std::move(variableName)),
          value(std::move(value)),
          ASTNode(line) {}

    void PrettyPrint(int indent = 0) override {
        std::string indentStr(indent, '\t');
        std::cout << indentStr << "LET " << variableName << std::endl;
        if (value) value->PrettyPrint(indent + 1);
    }

    void Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) override;
};

struct NumberNode : public ASTNode {
    double value;
    NumberNode(double value, int line) : value(value), ASTNode(line) {}

    void PrettyPrint(int indent = 0) override {
        std::string indentStr(indent, '\t');
        std::cout << indentStr << value << std::endl;
    }

    void Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) override;
};

struct StringNode : public ASTNode {
    std::string value;
    StringNode(std::string value, int line)
        : value(std::move(value)), ASTNode(line) {}
    void PrettyPrint(int indent = 0) override {
        std::string indentStr(indent, '\t');
        std::cout << indentStr << "\"" << value << "\"" << std::endl;
    };

    void Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) override;
};

struct VariableNode : public ASTNode {
    std::string name;
    VariableNode(std::string name, int line)
        : name(std::move(name)), ASTNode(line) {}

    void PrettyPrint(int indent = 0) override {
        std::string indentStr(indent, '\t');
        std::cout << indentStr << name << std::endl;
    }

    void Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) override;
};

struct BinaryOpNode : public ASTNode {
    TokenType op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(TokenType op, int opl, std::unique_ptr<ASTNode> left,
                 std::unique_ptr<ASTNode> right)
        : ASTNode(opl),
          op(op),
          left(std::move(left)),
          right(std::move(right)) {}

    void PrettyPrint(int indent = 0) override {
        std::string indentStr(indent, '\t');

        std::cout << indentStr << Util::TokenTypeToString(op) << std::endl;
        if (left) left->PrettyPrint(indent + 1);
        if (right) right->PrettyPrint(indent + 1);
    };

    void Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) override;
};

struct CallNode : public ASTNode {
    std::string functionName;
    std::vector<std::unique_ptr<ASTNode>> arguments;

    CallNode(std::string functionName,
             std::vector<std::unique_ptr<ASTNode>> arguments, int line)
        : functionName(std::move(functionName)),
          arguments(std::move(arguments)),
          ASTNode(line) {}

    void PrettyPrint(int indent = 0) override {
        std::string indentStr(indent, '\t');
        std::cout << indentStr << functionName << std::endl;
        for (const auto &arg : arguments) { arg->PrettyPrint(indent + 1); }
    }

    void Codegen(BytecodeCompiler &compiler, CompilerContext &ctx) override;
};

class Parser {
   public:
    Parser(Lexer &lexer)
        : lexer(lexer), currentToken(TokenType::UNKNOWN, -1) {
        Advance();
    }

    std::unique_ptr<ScopeNode> Parse();

   private:
    std::unique_ptr<ASTNode> ParseScope();
    std::unique_ptr<ASTNode> ParseStatement();
    std::unique_ptr<ASTNode> ParseExpression(int minPrecedence = 0);
    std::unique_ptr<ASTNode> ParsePrimary();
    std::unique_ptr<ASTNode> ParseCall(const std::string& functionName, int line);

    std::unique_ptr<ASTNode> ParseLetStatement();

    std::unique_ptr<ASTNode> ParseFunction();

    std::unique_ptr<ASTNode> ParseFunctionBody();

    void Advance() { currentToken = lexer.GetNextToken(); }
    void Expect(TokenType type);

    static int GetPrecedence(TokenType type);

   private:
    Lexer &lexer;
    Token currentToken;
};

}  // namespace EigerC

#endif  // _EIGERC_PARSER_HPP_