#ifndef _EIGERC_NUMBEROBJECT_HPP_
#define _EIGERC_NUMBEROBJECT_HPP_

#include <iomanip>
#include <sstream>

#include "DType.hpp"
#include "EiObject.hpp"
#include "Error.hpp"

namespace EigerC {

class NumberObject : public EiObject {
   public:
    NumberObject(int line, double val = 0) {
        this->line = line;
        value = val;
        type = DType::NUMBER;
    }

    double AsNumber() const override { return value; }

    std::string AsString() const override {
        std::ostringstream oss;
        oss << std::setprecision(15) << std::noshowpoint << value;
        return oss.str();
    }

    std::shared_ptr<EiObject> operator+(const EiObject &other) const override {
        if (other.type != DType::NUMBER) return EiObject::operator/(other);
        auto result = std::make_shared<NumberObject>(line);
        result->value = value + other.AsNumber();
        return result;
    }

    std::shared_ptr<EiObject> operator-(const EiObject &other) const override {
        if (other.type != DType::NUMBER) return EiObject::operator/(other);
        auto result = std::make_shared<NumberObject>(line);
        result->value = value - other.AsNumber();
        return result;
    }

    std::shared_ptr<EiObject> operator*(const EiObject &other) const override {
        if (other.type != DType::NUMBER) return EiObject::operator/(other);
        auto result = std::make_shared<NumberObject>(line);
        result->value = value * other.AsNumber();
        return result;
    }

    std::shared_ptr<EiObject> operator/(const EiObject &other) const override {
        if (other.type != DType::NUMBER) return EiObject::operator/(other);
        if (other.AsNumber() == 0.0) {
            throw Error(Error::Type::MATH_ERROR, "Zero division", line);
        }
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
