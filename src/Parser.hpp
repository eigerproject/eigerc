#ifndef _EIGERC_PARSER_HPP_
#define _EIGERC_PARSER_HPP_

#include <format>

#include "Lexer.hpp"
#include "Util.hpp"

namespace EigerC {

class Parser {
   public:
    Parser(Lexer &lexer)
        : m_Lexer(lexer), m_CurrentToken(TokenType::UNKNOWN, -1) {}

   private:
    void Advance() { m_CurrentToken = m_Lexer.GetNextToken(); }

    void Expect(TokenType type);

   private:
    Lexer &m_Lexer;
    Token m_CurrentToken;
};

}  // namespace EigerC

#endif  // _EIGERC_PARSER_HPP_