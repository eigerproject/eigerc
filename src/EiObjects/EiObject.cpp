#include "EiObject.hpp"

#include "BoolObject.hpp"

namespace EigerC {

std::shared_ptr<EiObject> EiObject::operator==(const EiObject &other) const {
    return std::make_shared<BoolObject>(line, false);
};

std::shared_ptr<EiObject> EiObject::operator!=(const EiObject &other) const {
    return std::make_shared<BoolObject>(line, true);
};

}  // namespace EigerC