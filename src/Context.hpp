#ifndef _EIGERC_COMPILER_CONTEXT_HPP_
#define _EIGERC_COMPILER_CONTEXT_HPP_

#include <string>
#include <unordered_map>

namespace EigerC {

enum class BuiltInFunctions { EMITLN = 0 };

class CompilerContext {
   public:
    int GetVariableID(const std::string& varName);
    int AddString(const std::string& str);
    std::string GetVarName(int id) const;

   public:
    std::unordered_map<int, std::string> stringTable;
    std::unordered_map<std::string, int> symbolTable = {
        {"emitln", static_cast<int>(BuiltInFunctions::EMITLN)}};
};

}  // namespace EigerC

#endif  // _EIGERC_COMPILER_CONTEXT_HPP_