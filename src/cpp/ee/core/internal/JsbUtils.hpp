#ifndef EE_X_JSB_UTILS_HPP
#define EE_X_JSB_UTILS_HPP

#include <string>

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
class JsbUtils {
public:
    static se::Object* getPath(se::Object* parent, const std::string& path);
    static se::Object* getChild(se::Object* parent, const std::string& name);
};
} // namespace core
} // namespace ee

#endif // EE_X_JSB_UTILS_HPP