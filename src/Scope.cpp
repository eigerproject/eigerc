#include "Scope.hpp"

#include <format>

#include "Error.hpp"

namespace EigerC {

void Scope::DeclareVar(int id) {
    values[id] = EiObject();
}

void Scope::SetVar(int id, EiObject value, int line) {
    ExpectVarExists(id, line);

    Scope *scope = this;

    while (scope) {
        if (scope->values.contains(id)) {
            scope->values[id] = value;
            break;
        }
        scope = scope->parent.get();
    }
}

void Scope::ExpectVarExists(int id, int line) {
    Scope *scope = this;

    while (scope) {
        if (scope->values.contains(id)) return;
        scope = scope->parent.get();
    }

    throw Error(Error::Type::RUNTIME_ERROR,
                std::format("Variable {} is not defined", ctx.GetVarName(id)),
                line);
}

EiObject Scope::GetVar(int id, int line) {
    ExpectVarExists(id, line);

    Scope *scope = this;

    while (scope) {
        if (scope->values.contains(id)) return scope->values[id];
        scope = scope->parent.get();
    }

    throw Error(Error::Type::RUNTIME_ERROR,
                std::format("Variable {} is not defined", ctx.GetVarName(id)),
                line);
}

}  // namespace EigerC