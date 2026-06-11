#include "ArrayObject.hpp"

#include <cmath>

#include "BoolObject.hpp"
#include "NumberObject.hpp"

std::string EigerC::ArrayObject::AsString() const {
    std::string result = "[";
    for (size_t i = 0; i < value.size(); ++i) {
        result += value[i]->AsString();
        if (i < value.size() - 1) result += ", ";
    }
    result += "]";
    return result;
}

std::shared_ptr<EigerC::EiObject> EigerC::ArrayObject::operator==(
    const EiObject &other) const {
    bool cond = true;

    if (other.type != DType::ARRAY)
        cond = false;
    else {
        const ArrayObject &otherArray = static_cast<const ArrayObject &>(other);

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

std::shared_ptr<EigerC::EiObject> EigerC::ArrayObject::operator[](
    const EigerC ::EiObject &other) const {
    if (other.type != DType::NUMBER) return EiObject::operator[](other);
    double idx = static_cast<const NumberObject &>(other).value;

    if (!std::isfinite(idx) || idx != std::floor(idx))
        throw Error(Error::Type::TYPE_ERROR,
                    "Arrays are only indexable by integer values", line);

    int iidx = idx;

    if (iidx < 0 || iidx >= value.size())
        throw Error(Error::Type::INDEX_ERROR, "Array index out of range", line);

    return value[static_cast<int>(idx)];
}
