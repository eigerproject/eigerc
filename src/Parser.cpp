#include "Parser.hpp"

#include "Error.hpp"

namespace EigerC {

std::unique_ptr<ScopeNode> Parser::Parse() {
    std::unique_ptr<ScopeNode> globalScope = std::make_unique<ScopeNode>(1);

    while (m_CurrentToken.type != TokenType::ENDOFFILE)
        globalScope->statements.push_back(std::move(ParseStatement()));

    return globalScope;
}

std::unique_ptr<ASTNode> Parser::ParseScope() {
    std::unique_ptr<ScopeNode> scope =
        std::make_unique<ScopeNode>(m_CurrentToken.line);

    Expect(TokenType::LBRACE);

    while (m_CurrentToken.type != TokenType::ENDOFFILE &&
           m_CurrentToken.type != TokenType::RBRACE)
        scope->statements.push_back(std::move(ParseStatement()));

    Expect(TokenType::RBRACE);

    return scope;
}

std::unique_ptr<ASTNode> Parser::ParseStatement() {
    if (m_CurrentToken.type == TokenType::LBRACE) return ParseScope();

    if (m_CurrentToken.type == TokenType::IDENTIFIER) {
        if (m_CurrentToken.lexeme == "let") return ParseLetStatement();
        if (m_CurrentToken.lexeme == "if") {
            Advance();
            auto condition = ParseExpression();
            auto ifBlock = ParseStatement();

            std::unique_ptr<ASTNode> elseBlock = nullptr;
            if (m_CurrentToken.type == TokenType::IDENTIFIER &&
                m_CurrentToken.lexeme == "else") {
                Advance();
                elseBlock = ParseStatement();
            }

            return std::make_unique<IfNode>(
                std::move(condition), std::move(ifBlock), m_CurrentToken.line,
                std::move(elseBlock));
        }
    }

    return ParseExpression();
}

std::unique_ptr<ASTNode> Parser::ParseExpression(int minPrecedence) {
    std::unique_ptr<ASTNode> left = ParsePrimary();
    if (!left) return NULL;

    while (true) {
        TokenType op = m_CurrentToken.type;
        int opLine = m_CurrentToken.line;
        int precedence = GetPrecedence(op);
        if (precedence < minPrecedence) break;

        Advance();
        auto right = ParseExpression(precedence + 1);
        if (!right) break;

        left = std::make_unique<BinaryOpNode>(op, opLine, std::move(left),
                                              std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::ParsePrimary() {
    if (m_CurrentToken.type == TokenType::NUMBER) {
        int line = m_CurrentToken.line;
        double val = std::stod(m_CurrentToken.lexeme);
        Advance();
        return std::make_unique<NumberNode>(val, line);
    }

    if (m_CurrentToken.type == TokenType::LPAREN) {
        Expect(TokenType::LPAREN);
        auto expr = ParseExpression();
        Expect(TokenType::RPAREN);
        return expr;
    }

    if (m_CurrentToken.type == TokenType::STRING) {
        int line = m_CurrentToken.line;

        std::string strValue = m_CurrentToken.lexeme;
        Advance();
        return std::make_unique<StringNode>(strValue, line);
    }

    if (m_CurrentToken.type == TokenType::IDENTIFIER) {
        int line = m_CurrentToken.line;

        std::string identValue = m_CurrentToken.lexeme;
        Advance();

        if (m_CurrentToken.type == TokenType::LPAREN)
            return ParseCall(identValue, line);

        return std::make_unique<VariableNode>(identValue, line);
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

std::unique_ptr<ASTNode> Parser::ParseCall(std::string functionName, int line) {
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
    return std::make_unique<CallNode>(functionName, std::move(arguments), line);
}

std::unique_ptr<ASTNode> Parser::ParseLetStatement() {
    Expect(TokenType::IDENTIFIER);
    int line = m_CurrentToken.line;
    std::string varName = m_CurrentToken.lexeme;
    Expect(TokenType::IDENTIFIER);

    std::unique_ptr<ASTNode> initialVal = nullptr;

    if (m_CurrentToken.type == TokenType::ASSIGN) {
        Expect(TokenType::ASSIGN);
        initialVal = ParseExpression();
        if (!initialVal) {
            throw Error(Error::Type::SYNTAX_ERROR,
                        "Expected expression after '='", m_CurrentToken.line);
        }
    }

    return std::make_unique<LetNode>(varName, std::move(initialVal), line);
}

int Parser::GetPrecedence(TokenType type) {
    switch (type) {
        case TokenType::PLUS:
        case TokenType::MINUS: return 1;
        case TokenType::MULTIPLY:
        case TokenType::DIVIDE: return 2;
        case TokenType::ASSIGN: return 0;
        default: return -1;
    }
}

}  // namespace EigerC