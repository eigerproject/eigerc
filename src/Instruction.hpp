#ifndef _EIGERC_BYTECODE_HPP_
#define _EIGERC_BYTECODE_HPP_

#include <format>
#include <iomanip>
#include <iostream>

#include "Opcode.hpp"
#include "Util.hpp"

namespace EigerC {

struct Instruction {
    Opcode opcode;
    double operand;
    bool flag = false;
    int sourceCodeLine;
    int address;

    Instruction(int addr, Opcode op, double opnd, bool flag, int srcln)
        : address(addr),
          opcode(op),
          operand(opnd),
          sourceCodeLine(srcln),
          flag(flag) {}

    void PrettyPrint() const {
        std::cout << std::format("{:05} {:<12} {:<10} {:<5}\n", address,
                                 Util::OpcodeToString(opcode), operand, flag);
    }

    static void PrettyPrintHeader() {
        std::cout << std::format("{:<5} {:<12} {:<10} {:<5}\n", "Addr",
                                 "Opcode", "Operand", "Flag");
    }
};

}  // namespace EigerC

#endif  // _EIGERC_BYTECODE_HPP_
