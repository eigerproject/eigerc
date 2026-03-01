#ifndef _EIGERC_EIOBJECT_HPP_
#define _EIGERC_EIOBJECT_HPP_

#include <format>
#include <memory>
#include <string>

#include "Context.hpp"
#include "DType.hpp"
#include "Error.hpp"
#include "Util.hpp"

namespace EigerC {

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

    virtual std::shared_ptr<EiObject> Attr(int attrId,
                                           CompilerContext &ctx) const {
        throw Error(Error::Type::ATTR_ERROR,
                    std::format("{} has no attribute {}",
                                Util::ObjectDTypeToString(type),
                                ctx.GetVarName(attrId)),
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

    virtual std::shared_ptr<EiObject> operator==(const EiObject &other) const;

    virtual std::shared_ptr<EiObject> operator!=(const EiObject &other) const;

    virtual std::shared_ptr<EiObject> operator<(const EiObject &other) const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("Operator `<` to object types {} and {}",
                                Util::ObjectDTypeToString(type),
                                Util::ObjectDTypeToString(other.type)),
                    line);
    };

    virtual std::shared_ptr<EiObject> operator>(const EiObject &other) const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("Operator `>` to object types {} and {}",
                                Util::ObjectDTypeToString(type),
                                Util::ObjectDTypeToString(other.type)),
                    line);
    };

    virtual std::shared_ptr<EiObject> operator<=(const EiObject &other) const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("Operator `<=` to object types {} and {}",
                                Util::ObjectDTypeToString(type),
                                Util::ObjectDTypeToString(other.type)),
                    line);
    };

    virtual std::shared_ptr<EiObject> operator>=(const EiObject &other) const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("Operator `>=` to object types {} and {}",
                                Util::ObjectDTypeToString(type),
                                Util::ObjectDTypeToString(other.type)),
                    line);
    };

    virtual operator bool() const { return true; };

    virtual std::shared_ptr<EiObject> operator!() const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("Unary `not` to object type {}",
                                Util::ObjectDTypeToString(type)),
                    line);
    };

    virtual std::shared_ptr<EiObject> operator-() const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("Unary `not` to object type {}",
                                Util::ObjectDTypeToString(type)),
                    line);
    };

    virtual std::shared_ptr<EiObject> operator[](const EiObject &other) const {
        throw Error(Error::Type::TYPE_ERROR,
                    std::format("Object type {} is not indexable by {}",
                                Util::ObjectDTypeToString(type),
                                Util::ObjectDTypeToString(other.type)),
                    line);
    };

    int line = -1;
    DType type = DType::UNKNOWN;
};

class NixObject : public EiObject {
   public:
    std::string AsString() const override { return "nix"; }
    operator bool() const override { return false; };
};

}  // namespace EigerC

#endif  // _EIGERC_EIOBJECT_HPP_
