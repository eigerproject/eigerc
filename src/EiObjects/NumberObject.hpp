#ifndef _EIGERC_NUMBEROBJECT_HPP_
#define _EIGERC_NUMBEROBJECT_HPP_

#include "EiObject.hpp"

namespace EigerC {

class NumberObject : public EiObject {
   public:
    NumberObject(int line, double val = 0) {
        this->line = line;
        value = val;
        type = DType::NUMBER;
    }

    double AsNumber() const override { return value; }
    std::string AsString() const override { return std::to_string(value); }

    std::shared_ptr<EiObject> operator+(const EiObject &other) const override {
        auto result = std::make_shared<NumberObject>(line);
        result->value = value + other.AsNumber();
        return result;
    }

    std::shared_ptr<EiObject> operator-(const EiObject &other) const override {
        auto result = std::make_shared<NumberObject>(line);
        result->value = value - other.AsNumber();
        return result;
    }

    std::shared_ptr<EiObject> operator*(const EiObject &other) const override {
        auto result = std::make_shared<NumberObject>(line);
        result->value = value * other.AsNumber();
        return result;
    }

    std::shared_ptr<EiObject> operator/(const EiObject &other) const override {
        auto result = std::make_shared<NumberObject>(line);
        result->value = value / other.AsNumber();
        return result;
    }

    virtual std::shared_ptr<EiObject> operator==(const EiObject &other) const;

    virtual std::shared_ptr<EiObject> operator!=(const EiObject &other) const;

    virtual std::shared_ptr<EiObject> operator<(const EiObject &other) const;

    virtual std::shared_ptr<EiObject> operator>(const EiObject &other) const;

    virtual std::shared_ptr<EiObject> operator<=(const EiObject &other) const;

    virtual std::shared_ptr<EiObject> operator>=(const EiObject &other) const;

    operator bool() const override { return value != 0.0; };

    double value = 0;
};

}  // namespace EigerC

#endif  // _EIGERC_NUMBEROBJECT_HPP_