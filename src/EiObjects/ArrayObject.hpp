#ifndef _EIGERC_ARRAYOBJECT_HPP_
#define _EIGERC_ARRAYOBJECT_HPP_

#include <cmath>
#include <memory>
#include <vector>

#include "BoolObject.hpp"
#include "DType.hpp"
#include "EiObject.hpp"
#include "NumberObject.hpp"

namespace EigerC {

class ArrayObject : public EiObject {
   public:
    ArrayObject(int line,
                const std::vector<std::shared_ptr<EiObject>>& elements) {
        this->line = line;
        value = elements;
        type = DType::ARRAY;
    }

    std::string AsString() const override {
        std::string result = "[";
        for (size_t i = 0; i < value.size(); ++i) {
            result += value[i]->AsString();
            if (i < value.size() - 1) result += ", ";
        }
        result += "]";
        return result;
    }

    std::shared_ptr<EiObject> operator==(const EiObject& other) const override {
        bool cond = true;

        if (other.type != DType::ARRAY)
            cond = false;
        else {
            const ArrayObject& otherArray =
                static_cast<const ArrayObject&>(other);

            if (value.size() != otherArray.value.size())
                cond = false;
            else {
                for (size_t i = 0; i < value.size(); ++i) {
                    auto result = (*(value[i]) == *(otherArray.value[i]));
                    auto boolObj = std::static_pointer_cast<BoolObject>(result);
                    if (!boolObj->value) cond = false;
                }
            }
        }

        return std::make_shared<BoolObject>(line, cond);
    }

    std::shared_ptr<EiObject> operator!=(const EiObject& other) const override {
        return !*(*this == other);
    }

    operator bool() const override { return value.size() > 0; };

    std::shared_ptr<EiObject> operator[](const EiObject& other) const override {
        if (other.type != DType::NUMBER) return EiObject::operator[](other);
        double idx = static_cast<const NumberObject&>(other).value;

        if (!std::isfinite(idx) || idx != std::floor(idx))
            throw Error(Error::Type::TYPE_ERROR,
                        "Arrays are only indexable by integer values", line);

        int iidx = idx;

        if (iidx < 0 || iidx >= value.size())
            throw Error(Error::Type::INDEX_ERROR, "Array index out of range",
                        line);

        return value[static_cast<int>(idx)];
    }

    std::vector<std::shared_ptr<EiObject>> value{};
};

}  // namespace EigerC

#endif  // _EIGERC_ARRAYOBJECT_HPP_
