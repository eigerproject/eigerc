#ifndef _EIGERC_UTIL_HPP_
#define _EIGERC_UTIL_HPP_

#include <string_view>

#include "Lexer.hpp"

namespace EigerC {
namespace Util {

inline std::string_view TokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::STRING: return "STRING";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::ENDOFFILE: return "<EOF>";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        default: return "UNKNOWN";
    }
}

}  // namespace Util
}  // namespace EigerC

#endif  // _EIGERC_UTIL_HPP_