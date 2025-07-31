#ifndef _EIGERC_ERROR_HPP_
#define _EIGERC_ERROR_HPP_

#include <string>

namespace EigerC {

class Error {
   public:
    enum class Type { UNKNOWN, SYNTAX_ERROR, NAME_ERROR, RUNTIME_ERROR };

    Error(Type type, const std::string& message, int line = -1)
        : m_Type(type), m_Message(message), m_Line(line) {}

    Type GetType() const { return m_Type; }
    const std::string& GetMessage() const { return m_Message; }
    int GetLine() const { return m_Line; }

   private:
    Type m_Type;
    std::string m_Message;
    int m_Line;
};

}  // namespace EigerC

#endif  // _EIGERC_ERROR_HPP_