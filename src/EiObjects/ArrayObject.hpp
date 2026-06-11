#ifndef _EIGERC_ARRAYOBJECT_HPP_
#define _EIGERC_ARRAYOBJECT_HPP_

#include <memory>
#include <vector>

#include "DType.hpp"
#include "EiObject.hpp"

namespace EigerC {

class ArrayObject : public EiObject {
   public:
    ArrayObject(int line,
                const std::vector<std::shared_ptr<EiObject>> &elements) {
        this->line = line;
        value = elements;
        type = DType::ARRAY;
    }

    std::string AsString() const override;

    std::shared_ptr<EiObject> operator==(const EiObject &other) const override;

    std::shared_ptr<EiObject> operator!=(const EiObject &other) const override {
        return !*(*this == other);
    }

    operator bool() const override { return value.size() > 0; };

    std::shared_ptr<EiObject> operator[](const EiObject &other) const override;

    std::vector<std::shared_ptr<EiObject>> value{};
};

}  // namespace EigerC

#endif  // _EIGERC_ARRAYOBJECT_HPP_
