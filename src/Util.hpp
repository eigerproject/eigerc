#ifndef _EIGERC_UTIL_HPP_
#define _EIGERC_UTIL_HPP_

#include <string_view>

#include "Error.hpp"
#include "Lexer.hpp"

namespace EigerC {

enum class Opcode;
enum class DType;

namespace Util {

std::string_view TokenTypeToString(TokenType type);

std::string_view ObjectDTypeToString(DType type);

std::string_view OpcodeToString(Opcode opcode);

std::string_view ErrorTypeToString(Error::Type type);

}  // namespace Util
}  // namespace EigerC

#endif  // _EIGERC_UTIL_HPP_