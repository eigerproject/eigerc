#include "Parser.hpp"

#include "Error.hpp"

namespace EigerC {

std::unique_ptr<ASTNode> Parser::Parse() {
    return Parser::ParseStatement();
}

std::unique_ptr<ASTNode> Parser::ParseStatement() {
    return ParseExpression();
}

std::unique_ptr<ASTNode> Parser::ParseExpression(int minPrecedence) {
    std::unique_ptr<ASTNode> left = ParsePrimary();
    if (!left) return NULL;

    while (true) {
        TokenType op = m_CurrentToken.type;
        int precedence = GetPrecedence(op);
        if (precedence < minPrecedence) break;

        Advance();
        auto right = ParseExpression(precedence + 1);
        if (!right) break;

        left = std::make_unique<BinaryOpNode>(op, std::move(left),
                                              std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::ParsePrimary() {
    if (m_CurrentToken.type == TokenType::NUMBER) {
        double val = std::stod(m_CurrentToken.lexeme);
        Advance();
        return std::make_unique<NumberNode>(val);
    }

    if (m_CurrentToken.type == TokenType::LPAREN) {
        Expect(TokenType::LPAREN);
        auto expr = ParseExpression();
        Expect(TokenType::RPAREN);
        return expr;
    }

    if (m_CurrentToken.type == TokenType::IDENTIFIER) {
        std::string identValue = m_CurrentToken.lexeme;
        Advance();

        if (m_CurrentToken.type == TokenType::LPAREN)
            return ParseCall(identValue);

        return std::make_unique<VariableNode>(identValue);
    }

    if (m_CurrentToken.type == TokenType::ENDOFFILE) return NULL;

    throw Error(Error::Type::SYNTAX_ERROR,
                std::format("Unexpected factor {}",
                            Util::TokenTypeToString(m_CurrentToken.type)),
                m_CurrentToken.line);
}

void Parser::Expect(TokenType type) {
    if (m_CurrentToken.type != type) {
        throw Error(Error::Type::SYNTAX_ERROR,
                    std::format("Unexpected token {}, expected {}",
                                Util::TokenTypeToString(m_CurrentToken.type),
                                Util::TokenTypeToString(type)),
                    m_CurrentToken.line);
    }
    Advance();
}

std::unique_ptr<ASTNode> Parser::ParseCall(std::string functionName) {
    Expect(TokenType::LPAREN);

    std::vector<std::unique_ptr<ASTNode>> arguments;
    while (m_CurrentToken.type != TokenType::RPAREN) {
        auto arg = ParseExpression();
        if (!arg) {
            throw Error(Error::Type::SYNTAX_ERROR,
                        "Expected expression in function call",
                        m_CurrentToken.line);
        }
        arguments.push_back(std::move(arg));

        if (m_CurrentToken.type == TokenType::COMMA) {
            Advance();
        } else if (m_CurrentToken.type != TokenType::RPAREN) {
            throw Error(Error::Type::SYNTAX_ERROR, "Expected ',' or ')'",
                        m_CurrentToken.line);
        }
    }

    Expect(TokenType::RPAREN);
    return std::make_unique<CallNode>(functionName, std::move(arguments));
}

int Parser::GetPrecedence(TokenType type) {
    switch (type) {
        case TokenType::PLUS:
        case TokenType::MINUS: return 1;
        case TokenType::MULTIPLY:
        case TokenType::DIVIDE: return 2;
        default: return -1;
    }
}

}  // namespace EigerC