cmake_minimum_required(VERSION 3.10)
project(eigerc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "Force gtest to use shared CRT" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(eigerc
    src/main.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/Compiler.cpp
    src/VM.cpp
    src/Util.cpp
    src/Context.cpp
    src/Scope.cpp
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
    src/CmdOpts.hpp

    src/EiObjects/EiObject.cpp
    src/EiObjects/NumberObject.cpp
    src/EiObjects/StringObject.cpp
    src/EiObjects/BoolObject.hpp
    src/EiObjects/DType.hpp
    src/EiObjects/EiObject.hpp
    src/EiObjects/FunctionObject.hpp
    src/EiObjects/FunctionObject.cpp
    src/EiObjects/NumberObject.hpp
    src/EiObjects/StringObject.hpp
    src/EiObjects/ArrayObject.hpp
)

add_executable(eigertest tests/runner.cpp)

add_custom_target(runAlways ALL)
add_custom_command(
    TARGET runAlways
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/tests"
            "$<TARGET_FILE_DIR:eigertest>/tests"
    COMMENT "Copying tests to eigertest build location"
)

target_link_libraries(eigertest gtest_main)

target_include_directories(${PROJECT_NAME} PRIVATE src src/EiObjects)
