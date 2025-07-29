#include <iostream>

#include "Error.hpp"
#include "Lexer.hpp"

int main() {
    std::string testSourceCode = "1 + 2 * 3";

    EigerC::Lexer lex(testSourceCode);

    EigerC::Token token(EigerC::TokenType::UNKNOWN, "");

    try {
        while ((token = lex.GetNextToken()).type !=
               EigerC::TokenType::ENDOFFILE) {
            std::cout << "Token: " << token.lexeme
                      << " Type: " << static_cast<int>(token.type) << "\n";
        }
    } catch (const EigerC::Error& e) {
        std::cerr << "Error: " << e.GetMessage() << " at line " << e.GetLine()
                  << "\n";
        return 1;
    }

    return 0;
}