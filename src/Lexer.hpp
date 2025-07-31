#ifndef _EIGERC_TOKEN_HPP_
#define _EIGERC_TOKEN_HPP_

#include <string>

namespace EigerC {

enum class TokenType {
    UNKNOWN,
    ENDOFFILE,
    IDENTIFIER,
    NUMBER,
    STRING,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    COMMA,
    ASSIGN,
    LBRACE,
    RBRACE
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;

    Token(TokenType t, int ln, const std::string& l = "")
        : type(t), lexeme(l), line(ln) {}
};

class Lexer {
   public:
    Lexer(const std::string& input);
    Token GetNextToken();

   private:
    void Advance();
    void SkipWhitespace();
    char Peek();

    Token Identifier();
    Token Number();
    Token String();

   private:
    int m_Line;

    std::string m_Input;
    size_t m_Pos;
    char m_CurrentChar;
};

}  // namespace EigerC

#endif  // _EIGERC_TOKEN_HPP_