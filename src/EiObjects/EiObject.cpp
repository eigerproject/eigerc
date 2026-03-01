#include "EiObject.hpp"

#include <memory>

#include "BoolObject.hpp"
#include "Context.hpp"
#include "FunctionObject.hpp"

namespace EigerC {

std::shared_ptr<EiObject> EiObject::operator==(const EiObject &other) const {
    return std::make_shared<BoolObject>(line, false);
};

std::shared_ptr<EiObject> EiObject::operator!=(const EiObject &other) const {
    return std::make_shared<BoolObject>(line, true);
};

std::shared_ptr<EiObject> EiObject::Attr(int attrId,
                                         CompilerContext &ctx) const {
    if (attrId == (int)CompilerContext::HardcodedSymbols::str) {
        return std::make_shared<Str>(
            std::const_pointer_cast<EigerC::EiObject>(shared_from_this()));
    }

    throw Error(
        Error::Type::ATTR_ERROR,
        std::format("{} has no attribute {}", Util::ObjectDTypeToString(type),
                    ctx.GetVarName(attrId)),
        line);
}

}  // namespace EigerC
