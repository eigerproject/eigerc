#include "Util.hpp"

#include "DType.hpp"
#include "Lexer.hpp"
#include "Opcode.hpp"

namespace EigerC {
namespace Util {

std::string_view TokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::UNKNOWN: return "UNKNOWN";
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
        case TokenType::COMMA: return "COMMA";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LSQRBRACE: return "LSQRBRACE";
        case TokenType::RSQRBRACE: return "RSQRBRACE";
        case TokenType::EQ: return "EQ";
        case TokenType::NEQ: return "NEQ";
        case TokenType::GT: return "GT";
        case TokenType::LT: return "LT";
        case TokenType::GTE: return "GTE";
        case TokenType::LTE: return "LTE";
        default: return "ADD_TO_STRING_CONVERSION";
    }
}

std::string_view ObjectDTypeToString(DType type) {
    switch (type) {
        case DType::UNKNOWN: return "UNKNOWN";
        case DType::STRING: return "STRING";
        case DType::NIX: return "NIX";
        case DType::NUMBER: return "NUMBER";
        case DType::FUNCTION: return "FUNCTION";
        default: return "ADD_TO_STRING_CONVERSION";
    }
}

std::string_view OpcodeToString(Opcode opcode) {
    switch (opcode) {
        case Opcode::NO_OP: return "NO_OP";
        case Opcode::LOAD_VAR: return "LOAD_VAR";
        case Opcode::LOAD_IMM: return "LOAD_IMM";
        case Opcode::LOAD_CONST: return "LOAD_CONST";
        case Opcode::LOAD_FUNC: return "LOAD_FUNC";
        case Opcode::STORE_VAR: return "STORE_VAR";
        case Opcode::POP_VAR: return "POP_VAR";
        case Opcode::DECL_VAR: return "DECL_VAR";
        case Opcode::NEW_SCOPE: return "NEW_SCOPE";
        case Opcode::END_SCOPE: return "END_SCOPE";
        case Opcode::CALL: return "CALL";
        case Opcode::TAIL_CALL: return "TAIL_CALL";
        case Opcode::RETURN: return "RETURN";
        case Opcode::ADD: return "ADD";
        case Opcode::SUBTRACT: return "SUBTRACT";
        case Opcode::MULTIPLY: return "MULTIPLY";
        case Opcode::DIVIDE: return "DIVIDE";
        case Opcode::JUMP: return "JUMP";
        case Opcode::JUMP_IF_FALSE: return "JUMP_IF_FALSE";
        case Opcode::EQUAL: return "EQUAL";
        case Opcode::NEQUAL: return "NEQUAL";
        case Opcode::GREATER: return "GREATER";
        case Opcode::LESS: return "LESS";
        case Opcode::GREATEREQ: return "GREATEREQ";
        case Opcode::LESSEQ: return "LESSEQ";
        case Opcode::MAKE_ARRAY: return "MAKE_ARRAY";
        case Opcode::INDEX: return "INDEX";
        default: return "ADD_TO_STRING_CONVERSION";
    }
}

std::string_view ErrorTypeToString(Error::Type type) {
    switch (type) {
        case Error::Type::UNKNOWN: return "UNKNOWN";
        case Error::Type::SYNTAX_ERROR: return "SYNTAX_ERROR";
        case Error::Type::NAME_ERROR: return "NAME_ERROR";
        case Error::Type::RUNTIME_ERROR: return "RUNTIME_ERROR";
        case Error::Type::TYPE_ERROR: return "TYPE_ERROR";
        case Error::Type::INTERNAL_ERROR: return "INTERNAL_ERROR";
        case Error::Type::ARG_ERROR: return "ARG_ERROR";
        case Error::Type::MATH_ERROR: return "MATH_ERROR";
        default: return "ADD_TO_STRING_CONVERSION";
    }
}

}  // namespace Util

}  // namespace EigerC
