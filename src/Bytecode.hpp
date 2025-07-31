#ifndef _EIGERC_BYTECODE_HPP_
#define _EIGERC_BYTECODE_HPP_

#include <iostream>

namespace EigerC {

enum Opcode {
    NO_OP,     // No Operation
    LOAD_VAR,  // Push variable value onto the stack

    LOAD_IMM,     // Push immediate value onto the stack
    LOAD_STRING,  // Push string constant onto the stack

    STORE_VAR,  // Pop value from the stack into a variable

    DECL_VAR,  // Declare a new variable in the current scope

    NEW_SCOPE,  // Create a new scope
    END_SCOPE,  // End the current scope

    CALL,  // Call a function

    ADD,       // Add top two values on the stack
    SUBTRACT,  // Subtract top two values on the stack
    MULTIPLY,  // Multiply top two values on the stack
    DIVIDE,    // Divide top two values on the stack
};

struct Instruction {
    Opcode opcode;
    double operand;
    int sourceCodeLine;

    Instruction(Opcode op, double opnd, int srcln)
        : opcode(op), operand(opnd), sourceCodeLine(srcln) {}

    void PrettyPrint() const {
        std::cout << "Opcode: " << static_cast<int>(opcode)
                  << ", Operand: " << operand << std::endl;
    }
};

}  // namespace EigerC

#endif  // _EIGERC_BYTECODE_HPP_