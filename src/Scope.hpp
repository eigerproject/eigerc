#ifndef _EIGERC_SCOPE_HPP_
#define _EIGERC_SCOPE_HPP_

#include <memory>
#include <unordered_map>
#include <utility>

#include "Context.hpp"
#include "EiObject.hpp"

namespace EigerC {

struct Scope {
    explicit Scope(CompilerContext &ctx,
                   std::shared_ptr<Scope> parent = nullptr)
        : parent(std::move(parent)), ctx(ctx) {}

    void DeclareVar(int id);
    void SetVar(int id, const std::shared_ptr<EiObject> &value, int line);
    void ExpectVarExists(int id, int line);
    std::shared_ptr<EiObject> GetVar(int id, int line);

    std::shared_ptr<Scope> parent = nullptr;
    std::unordered_map<int, std::shared_ptr<EiObject>> values{};

   private:
    CompilerContext &ctx;
};

}  // namespace EigerC

#endif  // _EIGERC_SCOPE_HPP_