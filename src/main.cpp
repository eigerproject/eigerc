#include <iostream>

#include "Error.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

int main() {
    std::string testSourceCode = "emitln(x * x + y * y)";

    try {
        EigerC::Lexer lex(testSourceCode);
        EigerC::Parser parser(lex);

        auto ast = parser.Parse();

        ast->PrettyPrint();
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