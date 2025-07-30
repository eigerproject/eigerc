#include <iostream>

#include "Error.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

int main() {
    std::string testSourceCode = "1 + 2 * 3";

    EigerC::Lexer lex(testSourceCode);
    EigerC::Parser parser(lex);

    return 0;
}