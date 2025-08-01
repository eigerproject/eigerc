#ifndef _EIGERC_EIOBJECT_HPP_
#define _EIGERC_EIOBJECT_HPP_

#include <sstream>
#include <string>
#include <variant>

namespace EigerC {

struct EiObject {
    std::variant<std::monostate, double, std::string> value;

    EiObject() = default;
    EiObject(double val) : value(val) {}
    EiObject(std::string val) : value(std::move(val)) {}

    std::string AsString() const {
        if (std::holds_alternative<std::monostate>(value)) return "nix";
        if (std::holds_alternative<double>(value)) {
            std::ostringstream oss;
            oss << std::defaultfloat << std::get<double>(value);
            return oss.str();
        }
        return std::get<std::string>(value);
    }

    bool IsTruthy() {
        if (std::holds_alternative<std::monostate>(value)) return false;
        if (std::holds_alternative<double>(value))
            return std::get<double>(value) != 0.0;
        if (std::holds_alternative<std::string>(value))
            return !std::get<std::string>(value).empty();
        return false;
    }

    EiObject Add(const EiObject& other) const {
        if (std::holds_alternative<std::monostate>(value) ||
            std::holds_alternative<std::monostate>(other.value))
            return EiObject();

        if (std::holds_alternative<double>(value) &&
            std::holds_alternative<double>(other.value))
            return EiObject(std::get<double>(value) +
                            std::get<double>(other.value));

        std::string lhs = AsString();
        std::string rhs = other.AsString();
        return EiObject(lhs + rhs);
    }

    template <typename F>
    EiObject BinaryNumericOp(const EiObject& other, F func) const {
        if (std::holds_alternative<double>(value) &&
            std::holds_alternative<double>(other.value)) {
            return EiObject(
                func(std::get<double>(value), std::get<double>(other.value)));
        }
        return EiObject();
    }

    EiObject Subtract(const EiObject& other) const {
        return BinaryNumericOp(other, std::minus<>());
    }

    EiObject Multiply(const EiObject& other) const {
        return BinaryNumericOp(other, std::multiplies<>());
    }

    EiObject Divide(const EiObject& other) const {
        if (std::holds_alternative<double>(value) &&
            std::holds_alternative<double>(other.value)) {
            double denom = std::get<double>(other.value);
            if (denom == 0.0) return EiObject();
            return EiObject(std::get<double>(value) / denom);
        }
        return EiObject();
    }
};

}  // namespace EigerC

#endif  // _EIGERC_EIOBJECT_HPP_