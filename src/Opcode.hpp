#ifndef _EIGERC_OPCODE_HPP_
#define _EIGERC_OPCODE_HPP_

namespace EigerC {

enum class Opcode {
    NO_OP,     // No Operation
    LOAD_VAR,  // Push variable value onto the stack

    LOAD_IMM,    // Push immediate value onto the stack
    LOAD_CONST,  // Push constant onto the stack
    LOAD_FUNC,   // Push function constant with attached closure

    POP_VAR,    // Pop value from the stack into a variable
    STORE_VAR,  // Store top value from the stack into a variable

    DECL_VAR,  // Declare a new variable in the current scope

    NEW_SCOPE,  // Create a new scope
    END_SCOPE,  // End the current scope

    CALL,       // Call a function + return flag
    RETURN,     // Return value
    TAIL_CALL,  // Tail call + return flag

    // Arithmetic between top two values on the stack
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,

    // Arithmetic for one value on the stack
    NOT,
    NEGATE,

    // Comparisons between top two values on the stack
    EQUAL,
    NEQUAL,
    GREATER,
    LESS,
    GREATEREQ,
    LESSEQ,

    JUMP_IF_FALSE,  // Jump if top of stack is falsy value
    JUMP,           // Unconditional jump

    MAKE_ARRAY,  // Make array of n items in stack

    INDEX,  // Index access
    ATTR,   // Attribute access
};

}

#endif  // _EIGERC_OPCODE_HPP_
