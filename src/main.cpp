#include <iostream>

#include "Compiler.hpp"
#include "Error.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

int main() {
    std::string testSourceCode = "let x = \"Hello World\"";

    try {
        EigerC::Lexer lex(testSourceCode);
        EigerC::Parser parser(lex);
        EigerC::BytecodeCompiler compiler(parser.Parse());

        const auto& bytecode = compiler.DoCodegen();

        for (const auto& instr : bytecode) { instr.PrettyPrint(); }

    } catch (EigerC::Error& e) {
        std::cerr << "Error: " << e.GetMessage() << " at line " << e.GetLine()
                  << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}