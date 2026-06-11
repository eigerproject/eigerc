#ifndef _EIGERC_BUILTINFUNCTIONS_HPP_
#define _EIGERC_BUILTINFUNCTIONS_HPP_

#include "FunctionObject.hpp"
#include "StringObject.hpp"

namespace EigerC {

class Str : public BuiltinFunctionObject {
   public:
    Str(std::shared_ptr<EiObject> obj)
        : obj(obj), BuiltinFunctionObject("str", {}) {}

    std::shared_ptr<EiObject> Execute(
        const std::vector<std::shared_ptr<EiObject>> &values) override {
        if (!convertedObj)
            convertedObj =
                std::make_shared<StringObject>(line, obj->AsString());

        return convertedObj;
    }

    std::shared_ptr<EiObject> obj, convertedObj;
};

}  // namespace EigerC

#endif  // _EIGERC_BUILTINFUNCTIONS_HPP_
