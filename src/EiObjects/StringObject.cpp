#include "StringObject.hpp"

#include "BoolObject.hpp"

namespace EigerC {

std::shared_ptr<EiObject> StringObject::operator==(
    const EiObject& other) const {
    return std::make_shared<BoolObject>(line, value == other.AsString());
}

std::shared_ptr<EiObject> StringObject::operator!=(
    const EiObject& other) const {
    return std::make_shared<BoolObject>(line, value != other.AsString());
}

}  // namespace EigerC