#ifndef _EIGERC_COMPILER_CONTEXT_HPP_
#define _EIGERC_COMPILER_CONTEXT_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "EiObject.hpp"

namespace EigerC {

class CompilerContext {
   public:
    CompilerContext();
    int GetVariableID(const std::string& varName);
    int AddConstant(const std::shared_ptr<EiObject>& c);
    std::string GetVarName(int id) const;

   public:
    std::unordered_map<int, std::shared_ptr<EiObject>> constantsTable;
    std::unordered_map<std::string, int> symbolTable;
};

}  // namespace EigerC

#endif  // _EIGERC_COMPILER_CONTEXT_HPP_