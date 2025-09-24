#ifndef _EIGERC_BOOLOBJECT_HPP_
#define _EIGERC_BOOLOBJECT_HPP_

#include "DType.hpp"
#include "EiObject.hpp"

namespace EigerC {

class BoolObject : public EiObject {
   public:
    BoolObject(int line, bool val = false) {
        this->line = line;
        value = val;
        type = DType::BOOL;
    }

    double AsNumber() const override { return (int)value; }
    std::string AsString() const override { return value ? "true" : "false"; }

    std::shared_ptr<EiObject> operator==(const EiObject &other) const override {
        return std::make_shared<BoolObject>(line, value == (bool)other);
    }

    std::shared_ptr<EiObject> operator!=(const EiObject &other) const override {
        return std::make_shared<BoolObject>(line, value != (bool)other);
    }

    operator bool() const override { return value; };

    bool value = false;
};

}  // namespace EigerC

#endif  // _EIGERC_BOOLOBJECT_HPP_