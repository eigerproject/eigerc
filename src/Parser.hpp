#ifndef _EIGERC_PARSER_HPP_
#define _EIGERC_PARSER_HPP_

#include <format>
#include <iostream>
#include <memory>

#include "Lexer.hpp"
#include "Util.hpp"

namespace EigerC {

struct ASTNode {
    virtual ~ASTNode() = default;

    virtual void PrettyPrint(int indent = 0) = 0;
};

struct NumberNode : public ASTNode {
    double value;
    NumberNode(double value) : value(value) {}

    void PrettyPrint(int indent = 0) override {
        std::string indentStr(indent, '\t');
        std::cout << indentStr << value << std::endl;
    }
};

struct BinaryOpNode : public ASTNode {
    TokenType op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(TokenType op, std::unique_ptr<ASTNode> left,
                 std::unique_ptr<ASTNode> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    void PrettyPrint(int indent = 0) override {
        std::string indentStr(indent, '\t');

        std::cout << indentStr << Util::TokenTypeToString(op) << std::endl;
        if (left) left->PrettyPrint(indent + 1);
        if (right) right->PrettyPrint(indent + 1);
    };
};

class Parser {
   public:
    Parser(Lexer &lexer)
        : m_Lexer(lexer), m_CurrentToken(TokenType::UNKNOWN, -1) {
        Advance();
    }

    std::unique_ptr<ASTNode> Parse();

   private:
    std::unique_ptr<ASTNode> ParseStatement();
    std::unique_ptr<ASTNode> ParseExpression(int minPrecedence = 0);
    std::unique_ptr<ASTNode> ParsePrimary();

    void Advance() { m_CurrentToken = m_Lexer.GetNextToken(); }
    void Expect(TokenType type);
    int GetPrecedence(TokenType type);

   private:
    Lexer &m_Lexer;
    Token m_CurrentToken;
};

}  // namespace EigerC

#endif  // _EIGERC_PARSER_HPP_