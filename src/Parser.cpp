#include "Parser.hpp"

#include "Error.hpp"

namespace EigerC {

std::unique_ptr<ScopeNode> Parser::Parse() {
    std::unique_ptr<ScopeNode> globalScope = std::make_unique<ScopeNode>(1);

    while (currentToken.type != TokenType::ENDOFFILE)
        globalScope->statements.push_back(std::move(ParseStatement()));

    return globalScope;
}

std::unique_ptr<ASTNode> Parser::ParseScope() {
    std::unique_ptr<ScopeNode> scope =
        std::make_unique<ScopeNode>(currentToken.line);

    Expect(TokenType::LBRACE);

    while (currentToken.type != TokenType::ENDOFFILE &&
           currentToken.type != TokenType::RBRACE)
        scope->statements.push_back(std::move(ParseStatement()));

    Expect(TokenType::RBRACE);

    return scope;
}

std::unique_ptr<ASTNode> Parser::ParseArray() {
    std::unique_ptr<ArrayNode> arr =
        std::make_unique<ArrayNode>(currentToken.line);

    Expect(TokenType::LSQRBRACE);

    while (currentToken.type != TokenType::ENDOFFILE &&
           currentToken.type != TokenType::RSQRBRACE) {
        arr->elements.push_back(std::move(ParseExpression()));

        if (currentToken.type != TokenType::COMMA) break;
        Expect(TokenType::COMMA);
    }

    Expect(TokenType::RSQRBRACE);
    return arr;
}

std::unique_ptr<ASTNode> Parser::ParseStatement() {
    std::unique_ptr<ASTNode> resultNode;

    if (currentToken.type == TokenType::LBRACE) {
        resultNode = ParseScope();
        resultNode->isAsStatement = true;
        return resultNode;
    }

    if (currentToken.type == TokenType::IDENTIFIER) {
        if (currentToken.lexeme == "let")
            return ParseLetStatement();
        else if (currentToken.lexeme == "if")
            return ParseIfStatement();
        else if (currentToken.lexeme == "ret")
            return ParseRetStatement();
    }

    resultNode = ParseExpression();
    resultNode->isAsStatement = true;
    return resultNode;
}

std::unique_ptr<ASTNode> Parser::ParseExpression(int minPrecedence) {
    std::unique_ptr<ASTNode> left = ParsePrimary();
    if (!left) return nullptr;

    while (true) {
        TokenType op = currentToken.type;
        int opLine = currentToken.line;
        int precedence = GetPrecedence(op);
        if (precedence < minPrecedence) break;

        bool isRightAssociative = (op == TokenType::ASSIGN);

        Advance();
        auto right =
            ParseExpression(isRightAssociative ? precedence : precedence + 1);
        if (!right) break;

        left = std::make_unique<BinaryOpNode>(op, opLine, std::move(left),
                                              std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::ParsePrimary() {
    switch (currentToken.type) {
        case TokenType::NUMBER: {
            int line = currentToken.line;
            double val = std::stod(currentToken.lexeme);
            Advance();
            return std::make_unique<NumberNode>(val, line);
        }
        case TokenType::LPAREN: {
            Expect(TokenType::LPAREN);
            auto expr = ParseExpression();
            Expect(TokenType::RPAREN);
            return expr;
        }
        case TokenType::STRING: {
            int line = currentToken.line;

            std::string strValue = currentToken.lexeme;
            Advance();
            return std::make_unique<StringNode>(strValue, line);
        }
        case TokenType::IDENTIFIER: {
            if (currentToken.lexeme == "func") return ParseFunction();

            int line = currentToken.line;

            std::string identValue = currentToken.lexeme;
            Advance();

            if (currentToken.type == TokenType::LPAREN)
                return ParseCall(identValue, line);

            return std::make_unique<VariableNode>(identValue, line);
        }
        case TokenType::LSQRBRACE: return ParseArray();
        case TokenType::ENDOFFILE: return nullptr;
    }

    throw Error(Error::Type::SYNTAX_ERROR,
                std::format("Unexpected factor {}",
                            Util::TokenTypeToString(currentToken.type)),
                currentToken.line);
}

void Parser::Expect(TokenType type) {
    if (currentToken.type != type) {
        throw Error(Error::Type::SYNTAX_ERROR,
                    std::format("Unexpected token {}, expected {}",
                                Util::TokenTypeToString(currentToken.type),
                                Util::TokenTypeToString(type)),
                    currentToken.line);
    }
    Advance();
}

std::unique_ptr<ASTNode> Parser::ParseCall(const std::string& functionName,
                                           int line) {
    Expect(TokenType::LPAREN);

    std::vector<std::unique_ptr<ASTNode>> arguments;
    while (currentToken.type != TokenType::RPAREN) {
        auto arg = ParseExpression();
        if (!arg) {
            throw Error(Error::Type::SYNTAX_ERROR,
                        "Expected expression in function call",
                        currentToken.line);
        }
        arguments.push_back(std::move(arg));

        if (currentToken.type == TokenType::COMMA) {
            Advance();
        } else if (currentToken.type != TokenType::RPAREN) {
            throw Error(Error::Type::SYNTAX_ERROR, "Expected ',' or ')'",
                        currentToken.line);
        }
    }

    Expect(TokenType::RPAREN);
    return std::make_unique<CallNode>(functionName, std::move(arguments), line);
}

std::unique_ptr<ASTNode> Parser::ParseRetStatement() {
    int line = currentToken.line;
    Advance();
    auto value = ParseExpression();

    auto result = std::make_unique<RetNode>(std::move(value), line);
    result->isAsStatement = true;
    return result;
}

std::unique_ptr<ASTNode> Parser::ParseIfStatement() {
    Advance();
    auto condition = ParseExpression();
    auto ifBlock = ParseStatement();

    std::unique_ptr<ASTNode> elseBlock = nullptr;
    if (currentToken.type == TokenType::IDENTIFIER &&
        currentToken.lexeme == "else") {
        Advance();
        elseBlock = ParseStatement();
    }

    auto result =
        std::make_unique<IfNode>(std::move(condition), std::move(ifBlock),
                                 currentToken.line, std::move(elseBlock));
    result->isAsStatement = true;
    return result;
}

std::unique_ptr<ASTNode> Parser::ParseLetStatement() {
    Expect(TokenType::IDENTIFIER);
    int line = currentToken.line;
    std::string varName = currentToken.lexeme;
    Expect(TokenType::IDENTIFIER);

    std::unique_ptr<ASTNode> initialVal = nullptr;

    if (currentToken.type == TokenType::ASSIGN) {
        Expect(TokenType::ASSIGN);
        initialVal = ParseExpression();
        if (!initialVal) {
            throw Error(Error::Type::SYNTAX_ERROR,
                        "Expected expression after '='", currentToken.line);
        }
    }

    auto result =
        std::make_unique<LetNode>(varName, std::move(initialVal), line);
    result->isAsStatement = true;

    return result;
}

std::unique_ptr<ASTNode> Parser::ParseFunction() {
    Expect(TokenType::IDENTIFIER);

    std::string functionName;
    int functionLine;

    std::vector<std::string> parameters;

    if (currentToken.type != TokenType::LPAREN) {
        functionName = currentToken.lexeme;
        functionLine = currentToken.line;
        Expect(TokenType::IDENTIFIER);
    }

    Expect(TokenType::LPAREN);

    while (currentToken.type != TokenType::RPAREN) {
        parameters.push_back(currentToken.lexeme);
        Expect(TokenType::IDENTIFIER);

        if (currentToken.type != TokenType::COMMA) break;
        Expect(TokenType::COMMA);
    }

    Expect(TokenType::RPAREN);

    bool isExpression = currentToken.type == TokenType::GT;
    std::unique_ptr<ASTNode> body = ParseFunctionBody();

    return std::make_unique<FunctionNode>(
        functionName, parameters, std::move(body), isExpression, functionLine);
}

std::unique_ptr<ASTNode> Parser::ParseFunctionBody() {
    if (currentToken.type == TokenType::GT) {
        Advance();
        return ParseExpression();
    } else if (currentToken.type == TokenType::LBRACE)
        return ParseScope();

    throw Error(Error::Type::SYNTAX_ERROR,
                "Expected `>` or `{` for function body");
}

int Parser::GetPrecedence(TokenType type) {
    switch (type) {
        case TokenType::ASSIGN: return 0;

        case TokenType::EQ:   // ==
        case TokenType::NEQ:  // !=
            return 1;

        case TokenType::LT:   // <
        case TokenType::LTE:  // <=
        case TokenType::GT:   // >
        case TokenType::GTE:  // >=
            return 2;

        case TokenType::PLUS:
        case TokenType::MINUS: return 3;

        case TokenType::MULTIPLY:
        case TokenType::DIVIDE: return 4;

        default: return -1;
    }
}

}  // namespace EigerC
