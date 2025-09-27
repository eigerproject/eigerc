cmake_minimum_required(VERSION 3.10)
project(eigerc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/Compiler.cpp
    src/VM.cpp
    src/Util.cpp
    src/Context.cpp
    src/Scope.cpp
    src/EiObjects/EiObject.cpp
    src/EiObjects/NumberObject.cpp
    src/EiObjects/StringObject.cpp

    src/EiObjects/BoolObject.hpp
    src/EiObjects/DType.hpp
    src/EiObjects/EiObject.hpp
    src/EiObjects/FunctionObject.hpp
    src/EiObjects/NumberObject.hpp
    src/EiObjects/StringObject.hpp
    src/Compiler.hpp
    src/Context.hpp
    src/Error.hpp
    src/Instruction.hpp
    src/Lexer.hpp
    src/Opcode.hpp
    src/Parser.hpp
    src/Scope.hpp
    src/Util.hpp
    src/VM.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src src/EiObjects)