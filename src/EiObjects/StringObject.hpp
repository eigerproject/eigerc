#ifndef _EIGERC_STRINGOBJECT_HPP_
#define _EIGERC_STRINGOBJECT_HPP_

#include "EiObject.hpp"
#include "Error.hpp"

namespace EigerC {

class StringObject : public EiObject {
   public:
    StringObject(int line) {
        this->line = line;
        type = DType::STRING;
    }
    StringObject(int line, std::string v) : StringObject(line) { value = v; }

    double AsNumber() const override { return std::stod(value); }
    std::string AsString() const override { return value; }

    std::shared_ptr<EiObject> operator+(const EiObject &other) const override {
        if (other.type != DType::STRING) return EiObject::operator+(other);

        auto result = std::make_shared<StringObject>(line);
        result->value = value + other.AsString();
        return result;
    }

    std::shared_ptr<EiObject> operator*(const EiObject &other) const override {
        if (other.type != DType::NUMBER) return EiObject::operator+(other);

        auto result = std::make_shared<StringObject>(line);
        int times = other.AsNumber();
        for (int i = 0; i < times; ++i) result->value += value;

        return result;
    }

    virtual std::shared_ptr<EiObject> operator==(const EiObject &other) const;

    virtual std::shared_ptr<EiObject> operator!=(const EiObject &other) const;

    std::string value;
};

}  // namespace EigerC

#endif  // _EIGERC_STRINGOBJECT_HPP_
