cmake_minimum_required(VERSION 3.10)
project(eigerc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Lexer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)