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
    DIVIDE
};

struct Token {
    TokenType type;
    std::string lexeme;

    Token(TokenType t, const std::string& l = "") : type(t), lexeme(l) {}
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