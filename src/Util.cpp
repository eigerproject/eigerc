#include "Util.hpp"

#include "Opcode.hpp"

namespace EigerC {
namespace Util {

std::string_view TokenTypeToString(TokenType type) {
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
        default: return "UNKNOWN_TOKEN";
    }
}

std::string_view OpcodeToString(Opcode opcode) {
    switch (opcode) {
        case Opcode::NO_OP: return "NO_OP";
        case Opcode::LOAD_VAR: return "LOAD_VAR";
        case Opcode::LOAD_IMM: return "LOAD_IMM";
        case Opcode::LOAD_STRING: return "LOAD_STRING";
        case Opcode::STORE_VAR: return "STORE_VAR";
        case Opcode::DECL_VAR: return "DECL_VAR";
        case Opcode::NEW_SCOPE: return "NEW_SCOPE";
        case Opcode::END_SCOPE: return "END_SCOPE";
        case Opcode::CALL: return "CALL_FUNC";
        case Opcode::ADD: return "ADD";
        case Opcode::SUBTRACT: return "SUBTRACT";
        case Opcode::MULTIPLY: return "MULTIPLY";
        case Opcode::DIVIDE: return "DIVIDE";
        default: return "UNKNOWN_OPCODE";
    }
}

std::string_view ErrorTypeToString(Error::Type type) {
    switch (type) {
        case Error::Type::UNKNOWN: return "UNKNOWN";
        case Error::Type::SYNTAX_ERROR: return "SYNTAX_ERROR";
        case Error::Type::NAME_ERROR: return "NAME_ERROR";
        default: return "UNRECOGNIZED_ERROR_TYPE";
    }
}

}  // namespace Util

}  // namespace EigerC