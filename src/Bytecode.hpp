#ifndef _EIGERC_BYTECODE_HPP_
#define _EIGERC_BYTECODE_HPP_

#include <iomanip>
#include <iostream>

#include "Opcode.hpp"
#include "Util.hpp"

namespace EigerC {

struct Instruction {
    Opcode opcode;
    double operand;
    int sourceCodeLine;
    int address;

    Instruction(int addr, Opcode op, double opnd, int srcln)
        : address(addr), opcode(op), operand(opnd), sourceCodeLine(srcln) {}

    void PrettyPrint() const {
        std::cout << std::setfill('0') << std::setw(5) << address << '\t';
        std::cout << Util::OpcodeToString(opcode) << '\t' << operand
                  << std::endl;
    }
};

}  // namespace EigerC

#endif  // _EIGERC_BYTECODE_HPP_