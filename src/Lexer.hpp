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
    EQ,
    NEQ,
    GT,
    LT,
    GTE,
    LTE,
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
    explicit Lexer(const std::string& input);
    Token GetNextToken();

   private:
    void Advance();
    void SkipWhitespace();
    char Peek();

    Token Identifier();
    Token Number();
    Token String();

   private:
    int line;

    std::string input;
    size_t pos;
    char currentChar;
};

}  // namespace EigerC

#endif  // _EIGERC_TOKEN_HPP_