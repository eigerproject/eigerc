#ifndef _EIGERC_ERROR_HPP_
#define _EIGERC_ERROR_HPP_

#include <string>

namespace EigerC {

class Error : public std::exception {
   public:
    enum class Type {
        UNKNOWN,
        ARG_ERROR,
        SYNTAX_ERROR,
        NAME_ERROR,
        RUNTIME_ERROR,
        TYPE_ERROR,
        INTERNAL_ERROR,
        MATH_ERROR,
        INDEX_ERROR,
        ATTR_ERROR,
    };

    Error(Type type, const std::string &message, int line = -1)
        : type(type), message(message), line(line) {}

    [[nodiscard]] Type GetType() const { return type; }
    [[nodiscard]] const std::string &GetMessage() const { return message; }
    [[nodiscard]] int GetLine() const { return line; }

   private:
    Type type;
    std::string message;
    int line;
};

}  // namespace EigerC

#endif  // _EIGERC_ERROR_HPP_
