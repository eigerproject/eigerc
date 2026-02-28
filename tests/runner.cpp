#include <gtest/gtest.h>

#include <cstdio>
#include <string>

#if defined(_MSC_VER)
#define popen _popen
#define pclose _pclose
#endif

std::string runInterpreter(const std::string& file) {
#if defined(_WIN32)
    std::string command = "eigerc -s \"" + file + "\"";
#else
    std::string command = "./eigerc -s \"" + file + "\"";
#endif

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "";

    char buffer[128];
    std::string result;

    while (fgets(buffer, sizeof(buffer), pipe)) result += buffer;

    pclose(pipe);
    return result;
}

std::string readFile(const std::string& file) {
    std::string content = "";
    FILE* filePtr = fopen(file.c_str(), "r");
    if (!filePtr) return content;

    char buffer[128];
    while (!feof(filePtr)) {
        if (fgets(buffer, 128, filePtr) != NULL) content += buffer;
    }
    fclose(filePtr);
    return content;
}

#define TEST_FILE(suite, name, path)                          \
    TEST(suite, name) {                                       \
        auto content = runInterpreter(path);                  \
        auto expected = readFile(std::string(path) + ".out"); \
        EXPECT_EQ(content, expected);                         \
    }

TEST_FILE(IntegrationTests, Arithmetic, "tests/arithmetic.eig");
TEST_FILE(IntegrationTests, Variables, "tests/variables.eig");
