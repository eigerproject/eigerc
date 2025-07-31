#include "Lexer.hpp"

#include <format>

#include "Error.hpp"

namespace EigerC {

Lexer::Lexer(const std::string& input) {
    m_Input = input;
    m_Pos = 0;
    m_CurrentChar = m_Input.empty() ? '\0' : m_Input[m_Pos];

    m_Line = 1;
}

void Lexer::Advance() {
    if (m_CurrentChar == '\n') m_Line++;

    m_Pos++;

    if (m_Pos >= m_Input.size()) {
        m_CurrentChar = '\0';
    } else {
        m_CurrentChar = m_Input[m_Pos];
    }
}

void Lexer::SkipWhitespace() {
    while (m_CurrentChar != '\0' && isspace(m_CurrentChar)) Advance();
}

char Lexer::Peek() {
    if (m_Pos + 1 < m_Input.size()) return m_Input[m_Pos + 1];
    return '\0';
}

Token Lexer::Identifier() {
    std::string result;
    while (m_CurrentChar != '\0' &&
           (isalnum(m_CurrentChar) || m_CurrentChar == '_')) {
        result += m_CurrentChar;
        Advance();
    }
    return Token(TokenType::IDENTIFIER, m_Line, result);
}

Token Lexer::Number() {
    std::string result;
    while (m_CurrentChar != '\0' &&
           (isdigit(m_CurrentChar) || m_CurrentChar == '.')) {
        result += m_CurrentChar;
        Advance();
    }
    return Token(TokenType::NUMBER, m_Line, result);
}

Token Lexer::String() {
    std::string result;
    if (m_CurrentChar != '"')
        throw Error(Error::Type::SYNTAX_ERROR, std::format("Expected '\"'"),
                    m_Line);

    Advance();
    while (m_CurrentChar != '\0' && m_CurrentChar != '"') {
        if (m_CurrentChar == '\\') {
            Advance();
            if (m_CurrentChar == 'n') {
                result += '\n';
            } else if (m_CurrentChar == 't') {
                result += '\t';
            } else if (m_CurrentChar == '"') {
                result += '"';
            } else {
                result += m_CurrentChar;  // Add the escaped character
            }
        } else {
            result += m_CurrentChar;
        }
        Advance();
    }

    if (m_CurrentChar != '"') {
        throw Error(Error::Type::SYNTAX_ERROR,
                    std::format("Unterminated string literal"), m_Line);
    }

    Advance();

    return Token(TokenType::STRING, m_Line, result);
}

Token Lexer::GetNextToken() {
    while (m_CurrentChar != '\0') {
        if (isspace(m_CurrentChar)) {
            SkipWhitespace();
            continue;
        }
        if (isalpha(m_CurrentChar) || m_CurrentChar == '_') return Identifier();
        if (isdigit(m_CurrentChar)) return Number();

        switch (m_CurrentChar) {
            case '+': Advance(); return Token(TokenType::PLUS, m_Line);
            case '-': Advance(); return Token(TokenType::MINUS, m_Line);
            case '*': Advance(); return Token(TokenType::MULTIPLY, m_Line);
            case '/': Advance(); return Token(TokenType::DIVIDE, m_Line);
            case '(': Advance(); return Token(TokenType::LPAREN, m_Line);
            case ')': Advance(); return Token(TokenType::RPAREN, m_Line);
            case ',': Advance(); return Token(TokenType::COMMA, m_Line);
            case '=': Advance(); return Token(TokenType::ASSIGN, m_Line);
            case '{': Advance(); return Token(TokenType::LBRACE, m_Line);
            case '}': Advance(); return Token(TokenType::RBRACE, m_Line);
            case '"': return String();
            default:
                throw Error(
                    Error::Type::SYNTAX_ERROR,
                    std::format("Unexpected character: `{}`", m_CurrentChar),
                    m_Line);
        }
    }
    return Token(TokenType::ENDOFFILE, m_Line);
}

}  // namespace EigerC