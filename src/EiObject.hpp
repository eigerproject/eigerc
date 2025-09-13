#ifndef _EIGERC_EIOBJECT_HPP_
#define _EIGERC_EIOBJECT_HPP_

#include <format>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <variant>

#include "Error.hpp"
#include "Util.hpp"

namespace EigerC {

enum class DType { UNKNOWN, NUMBER, STRING, NIX };

class EiObject {
   public:
    EiObject() = default;
    virtual ~EiObject() = default;

    virtual std::string AsString() const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("{} cannot implicitly be converted to string",
                                Util::ObjectDTypeToString(type)),
                    line);
    }

    virtual double AsNumber() const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("{} cannot implicitly be converted to number",
                                Util::ObjectDTypeToString(type)),
                    line);
    }

    virtual std::shared_ptr<EiObject> operator+(const EiObject &other) const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("Operator `+` to object types {} and {}",
                                Util::ObjectDTypeToString(type),
                                Util::ObjectDTypeToString(other.type)),
                    line);
    };

    virtual std::shared_ptr<EiObject> operator-(const EiObject &other) const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("Operator `-` to object types {} and {}",
                                Util::ObjectDTypeToString(type),
                                Util::ObjectDTypeToString(other.type)),
                    line);
    };

    virtual std::shared_ptr<EiObject> operator*(const EiObject &other) const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("Operator `*` to object types {} and {}",
                                Util::ObjectDTypeToString(type),
                                Util::ObjectDTypeToString(other.type)),
                    line);
    };

    virtual std::shared_ptr<EiObject> operator/(const EiObject &other) const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("Operator `+` to object types {} and {}",
                                Util::ObjectDTypeToString(type),
                                Util::ObjectDTypeToString(other.type)),
                    line);
    };

    virtual operator bool() const { return true; };

    int line = -1;
    DType type = DType::UNKNOWN;
};

class NixObject : public EiObject {
   public:
    std::string AsString() const override { return "nix"; }
    operator bool() const override { return false; };
};

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

    operator bool() const override { return value != 0.0; };

    double value = 0;
};

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
        auto result = std::make_shared<StringObject>(line);
        result->value = value + other.AsString();
        return result;
    }

    std::shared_ptr<EiObject> operator*(const EiObject &other) const override {
        auto result = std::make_shared<StringObject>(line);
        int times = other.AsNumber();
        for (int i = 0; i < times; ++i) result->value += value;

        return result;
    }

    std::string value;
};

}  // namespace EigerC

#endif  // _EIGERC_EIOBJECT_HPP_