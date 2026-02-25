#include "NumberObject.hpp"

#include "BoolObject.hpp"

namespace EigerC {

std::shared_ptr<EiObject> NumberObject::operator==(
    const EiObject &other) const {
    if (other.type != DType::NUMBER)
        return std::make_shared<BoolObject>(line, false);

    return std::make_shared<BoolObject>(line, value == other.AsNumber());
};

std::shared_ptr<EiObject> NumberObject::operator!=(
    const EiObject &other) const {
    if (other.type != DType::NUMBER)
        return std::make_shared<BoolObject>(line, true);

    return std::make_shared<BoolObject>(line, value != other.AsNumber());
};

std::shared_ptr<EiObject> NumberObject::operator<(const EiObject &other) const {
    if (other.type != DType::NUMBER) return EiObject::operator<(other);
    return std::make_shared<BoolObject>(line, value < other.AsNumber());
};

std::shared_ptr<EiObject> NumberObject::operator>(const EiObject &other) const {
    if (other.type != DType::NUMBER) return EiObject::operator>(other);
    return std::make_shared<BoolObject>(line, value > other.AsNumber());
};

std::shared_ptr<EiObject> NumberObject::operator<=(
    const EiObject &other) const {
    if (other.type != DType::NUMBER) return EiObject::operator<=(other);
    return std::make_shared<BoolObject>(line, value <= other.AsNumber());
};

std::shared_ptr<EiObject> NumberObject::operator>=(
    const EiObject &other) const {
    if (other.type != DType::NUMBER) return EiObject::operator>=(other);
    return std::make_shared<BoolObject>(line, value >= other.AsNumber());
};

}  // namespace EigerC
