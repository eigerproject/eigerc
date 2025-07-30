#include "Parser.hpp"

#include "Error.hpp"

namespace EigerC {

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

}  // namespace EigerC