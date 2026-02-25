#include "StringObject.hpp"

#include "BoolObject.hpp"

namespace EigerC {

std::shared_ptr<EiObject> StringObject::operator==(
    const EiObject& other) const {
    if (other.type != DType::STRING)
        return std::make_shared<BoolObject>(line, false);

    return std::make_shared<BoolObject>(line, value == other.AsString());
}

std::shared_ptr<EiObject> StringObject::operator!=(
    const EiObject& other) const {
    if (other.type != DType::STRING)
        return std::make_shared<BoolObject>(line, true);

    return std::make_shared<BoolObject>(line, value != other.AsString());
}

}  // namespace EigerC
