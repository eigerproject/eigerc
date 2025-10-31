#include "Lexer.hpp"

#include <format>

#include "Error.hpp"

namespace EigerC {

Lexer::Lexer(const std::string& input) {
    this->input = input;
    pos = 0;
    currentChar = input.empty() ? '\0' : input[pos];

    line = 1;
}

void Lexer::Advance() {
    if (currentChar == '\n') line++;

    pos++;

    if (pos >= input.size()) {
        currentChar = '\0';
    } else {
        currentChar = input[pos];
    }
}

void Lexer::SkipWhitespace() {
    while (currentChar != '\0' && isspace(currentChar)) Advance();
}

char Lexer::Peek() {
    if (pos + 1 < input.size()) return input[pos + 1];
    return '\0';
}

Token Lexer::Identifier() {
    std::string result;
    while (currentChar != '\0' &&
           (isalnum(currentChar) || currentChar == '_')) {
        result += currentChar;
        Advance();
    }
    return {TokenType::IDENTIFIER, line, result};
}

Token Lexer::Number() {
    std::string result;
    while (currentChar != '\0' &&
           (isdigit(currentChar) || currentChar == '.')) {
        result += currentChar;
        Advance();
    }
    return {TokenType::NUMBER, line, result};
}

Token Lexer::String() {
    std::string result;
    if (currentChar != '"')
        throw Error(Error::Type::SYNTAX_ERROR, std::format("Expected '\"'"),
                    line);

    Advance();
    while (currentChar != '\0' && currentChar != '"') {
        if (currentChar == '\\') {
            Advance();
            if (currentChar == 'n') {
                result += '\n';
            } else if (currentChar == 't') {
                result += '\t';
            } else if (currentChar == '"') {
                result += '"';
            } else {
                result += currentChar;  // Add the escaped character
            }
        } else {
            result += currentChar;
        }
        Advance();
    }

    if (currentChar != '"') {
        throw Error(Error::Type::SYNTAX_ERROR,
                    std::format("Unterminated string literal"), line);
    }

    Advance();

    return {TokenType::STRING, line, result};
}

Token Lexer::GetNextToken() {
    while (currentChar != '\0') {
        if (isspace(currentChar)) {
            SkipWhitespace();
            continue;
        }
        if (isalpha(currentChar) || currentChar == '_') return Identifier();
        if (isdigit(currentChar)) return Number();

        if (currentChar == '~') {
            while (currentChar != '\0' && currentChar != '\n') Advance();
            SkipWhitespace();
            continue;
        }

        switch (currentChar) {
            case '+': Advance(); return {TokenType::PLUS, line};
            case '-': Advance(); return {TokenType::MINUS, line};
            case '*': Advance(); return {TokenType::MULTIPLY, line};
            case '/': Advance(); return {TokenType::DIVIDE, line};
            case '(': Advance(); return {TokenType::LPAREN, line};
            case ')': Advance(); return {TokenType::RPAREN, line};
            case ',': Advance(); return {TokenType::COMMA, line};
            case '{': Advance(); return {TokenType::LBRACE, line};
            case '}': Advance(); return {TokenType::RBRACE, line};
            case '[': Advance(); return {TokenType::LSQRBRACE, line};
            case ']': Advance(); return {TokenType::RSQRBRACE, line};
            case '=': {
                Advance();
                return {TokenType::ASSIGN, line};
            }
            case '!': {
                Advance();
                if (currentChar != '=') {
                    throw Error(Error::Type::SYNTAX_ERROR,
                                std::format("Unexpected character: `{}`", '!'),
                                line);
                }
                Advance();
                return {TokenType::NEQ, line};
            }
            case '?': {
                Advance();
                if (currentChar != '=') {
                    throw Error(Error::Type::SYNTAX_ERROR,
                                std::format("Unexpected character: `{}`", '?'),
                                line);
                }
                Advance();
                return {TokenType::EQ, line};
            }
            case '<': {
                Advance();
                if (currentChar == '=') {
                    Advance();
                    return {TokenType::LTE, line};
                }
                return {TokenType::LT, line};
            }
            case '>': {
                Advance();
                if (currentChar == '=') {
                    Advance();
                    return {TokenType::GTE, line};
                }
                return {TokenType::GT, line};
            }
            case '"': return String();
            default:
                throw Error(
                    Error::Type::SYNTAX_ERROR,
                    std::format("Unexpected character: `{}`", currentChar),
                    line);
        }
    }
    return {TokenType::ENDOFFILE, line};
}

}  // namespace EigerC