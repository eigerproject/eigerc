#ifndef _EIGERC_BYTECODE_HPP_
#define _EIGERC_BYTECODE_HPP_

#include <iostream>

#include "Opcode.hpp"
#include "Util.hpp"

namespace EigerC {

struct Instruction {
    Opcode opcode;
    double operand;
    int sourceCodeLine;

    Instruction(Opcode op, double opnd, int srcln)
        : opcode(op), operand(opnd), sourceCodeLine(srcln) {}

    void PrettyPrint() const {
        std::cout << Util::OpcodeToString(opcode) << '\t' << operand
                  << std::endl;
    }
};

}  // namespace EigerC

#endif  // _EIGERC_BYTECODE_HPP_