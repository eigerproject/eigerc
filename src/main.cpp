#include <iostream>

#include "Compiler.hpp"
#include "Error.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "VM.hpp"

int main() {
    std::string testSourceCode = "emitln(1 + 2 * 3)";

    try {
        EigerC::Lexer lex(testSourceCode);
        EigerC::Parser parser(lex);
        EigerC::BytecodeCompiler compiler(parser.Parse());
        compiler.DoCodegen();

        EigerC::BytecodeVM vm(compiler);

        vm.ExecuteBytecode();

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