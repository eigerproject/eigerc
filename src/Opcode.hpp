#ifndef _EIGERC_OPCODE_HPP_
#define _EIGERC_OPCODE_HPP_

namespace EigerC {

enum class Opcode {
    NO_OP,     // No Operation
    LOAD_VAR,  // Push variable value onto the stack

    LOAD_IMM,    // Push immediate value onto the stack
    LOAD_CONST,  // Push constant onto the stack

    POP_VAR,    // Pop value from the stack into a variable
    STORE_VAR,  // Store top value from the stack into a variable

    DECL_VAR,  // Declare a new variable in the current scope

    NEW_SCOPE,  // Create a new scope
    END_SCOPE,  // End the current scope

    CALL,  // Call a function

    ADD,       // Add top two values on the stack
    SUBTRACT,  // Subtract top two values on the stack
    MULTIPLY,  // Multiply top two values on the stack
    DIVIDE,    // Divide top two values on the stack

    JUMP_IF_FALSE,  // Jump if top of stack is falsy value
    JUMP,           // Unconditional jump
};

}

#endif  // _EIGERC_OPCODE_HPP_