#include "Context.hpp"

namespace EigerC {

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

int CompilerContext::AddString(const std::string& str) {
    int id = stringTable.size();
    stringTable[id] = str;
    return id;
}

}  // namespace EigerC