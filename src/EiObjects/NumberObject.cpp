#include "NumberObject.hpp"

#include "BoolObject.hpp"

namespace EigerC {

std::shared_ptr<EiObject> NumberObject::operator==(
    const EiObject &other) const {
    return std::make_shared<BoolObject>(line, value == other.AsNumber());
};

std::shared_ptr<EiObject> NumberObject::operator!=(
    const EiObject &other) const {
    return std::make_shared<BoolObject>(line, value != other.AsNumber());
};

std::shared_ptr<EiObject> NumberObject::operator<(const EiObject &other) const {
    return std::make_shared<BoolObject>(line, value < other.AsNumber());
};

std::shared_ptr<EiObject> NumberObject::operator>(const EiObject &other) const {
    return std::make_shared<BoolObject>(line, value > other.AsNumber());
};

std::shared_ptr<EiObject> NumberObject::operator<=(
    const EiObject &other) const {
    return std::make_shared<BoolObject>(line, value <= other.AsNumber());
};

std::shared_ptr<EiObject> NumberObject::operator>=(
    const EiObject &other) const {
    return std::make_shared<BoolObject>(line, value >= other.AsNumber());
};

}  // namespace EigerC