#include "Context.hpp"

#include "FunctionObject.hpp"

namespace EigerC {

CompilerContext::CompilerContext() {}

int CompilerContext::GetVariableID(const std::string& varName) {
    if (symbolTable.contains(varName)) return symbolTable[varName];
    int newID = static_cast<int>(symbolTable.size());
    symbolTable[varName] = newID;
    return newID;
}

std::string CompilerContext::GetVarName(int id) const {
    for (const auto& [name, varID] : symbolTable) {
        if (varID == id) return name;
    }
    return "<UNKNOWN_VAR>";
}

int CompilerContext::AddConstant(const std::shared_ptr<EiObject>& c) {
    int id = static_cast<int>(constantsTable.size());
    constantsTable[id] = c;
    return id;
}

}  // namespace EigerC