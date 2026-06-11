#include "EiObject.hpp"

#include <memory>

#include "BoolObject.hpp"
#include "BuiltinFunctions.h"
#include "Context.hpp"

namespace EigerC {

std::shared_ptr<EiObject> EiObject::operator==(const EiObject &other) const {
    return std::make_shared<BoolObject>(line, false);
};

std::shared_ptr<EiObject> EiObject::operator!=(const EiObject &other) const {
    return std::make_shared<BoolObject>(line, true);
};

std::shared_ptr<EiObject> EiObject::Attr(int attrId, CompilerContext &ctx) {
    auto it = attributes.find(attrId);
    if (it != attributes.end()) return it->second;

    const auto &table = GetAttrTable();
    auto factoryIt = table.find(attrId);
    if (factoryIt != table.end()) {
        auto attr = factoryIt->second(shared_from_this());
        return attributes.emplace(attrId, std::move(attr)).first->second;
    }

    throw Error(
        Error::Type::ATTR_ERROR,
        std::format("{} has no attribute {}", Util::ObjectDTypeToString(type),
                    ctx.GetVarName(attrId)),
        line);
}

const std::unordered_map<int, AttrFactory> &EiObject::BaseAttrTable() {
    using HS = CompilerContext::HardcodedSymbols;
    static const std::unordered_map<int, AttrFactory> table = {
        {(int)HS::str, [](auto self) { return std::make_shared<Str>(self); }},
    };
    return table;
}

}  // namespace EigerC
