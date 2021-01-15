#ifndef EE_X_JSB_UTILS_HPP
#define EE_X_JSB_UTILS_HPP

#include <string>

#include "ee/core/JsbTemplates.hpp"

namespace ee {
namespace core {
class JsbUtils {
public:
    template <class Instance, class... Args>
    static bool makeConstructor(se::State& state) {
        return ee::core::makeConstructor<Instance, Args...>(state);
    }

    template <class T>
    static bool makeFinalize(se::State& state) {
        return ee::core::makeFinalize<T>(state);
    }

    template <auto Function>
    static bool makeMethod(se::State& state) {
        return ee::core::makeMethod<Function>(state);
    }

    static se::Object* getPath(se::Object* parent, const std::string& path);
    static se::Object* getChild(se::Object* parent, const std::string& name);
};
} // namespace core
} // namespace ee

#endif // EE_X_JSB_UTILS_HPP