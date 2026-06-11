#ifndef _EIGERC_BUILTINFUNCTIONS_HPP_
#define _EIGERC_BUILTINFUNCTIONS_HPP_

#include "ArrayObject.hpp"
#include "FunctionObject.hpp"
#include "StringObject.hpp"

namespace EigerC {

class Str : public BuiltinFunctionObject {
   public:
    Str(std::shared_ptr<EiObject> obj)
        : obj(obj), BuiltinFunctionObject("str", {}, false) {}

    std::shared_ptr<EiObject> Execute(
        const std::vector<std::shared_ptr<EiObject>> &values) override {
        if (!convertedObj)
            convertedObj =
                std::make_shared<StringObject>(line, obj->AsString());

        return convertedObj;
    }

    std::shared_ptr<EiObject> obj, convertedObj;
};

class Emitln : public BuiltinFunctionObject {
   public:
    Emitln() : BuiltinFunctionObject("emitln", {"values"}, true) {}
    std::shared_ptr<EiObject> Execute(
        const std::vector<std::shared_ptr<EiObject>> &args) override {
        std::shared_ptr<ArrayObject> values =
            std::static_pointer_cast<ArrayObject>(args[0]);

        size_t count = values->value.size();

        for (size_t i = 0; i < count; ++i)
            std::cout << values->value[i]->AsString()
                      << ((i == count - 1) ? '\n' : ' ');

        return std::make_shared<NixObject>();
    }
};

}  // namespace EigerC

#endif  // _EIGERC_BUILTINFUNCTIONS_HPP_
