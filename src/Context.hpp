#ifndef _EIGERC_COMPILER_CONTEXT_HPP_
#define _EIGERC_COMPILER_CONTEXT_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "EiObject.hpp"

namespace EigerC {

enum class BuiltInFunctions { EMITLN = 0 };

class CompilerContext {
   public:
    int GetVariableID(const std::string& varName);
    int AddConstant(const std::shared_ptr<EiObject>& c);
    std::string GetVarName(int id) const;

   public:
    std::unordered_map<int, std::shared_ptr<EiObject>> constantsTable;
    std::unordered_map<std::string, int> symbolTable = {
        {"emitln", static_cast<int>(BuiltInFunctions::EMITLN)}};
};

}  // namespace EigerC

#endif  // _EIGERC_COMPILER_CONTEXT_HPP_