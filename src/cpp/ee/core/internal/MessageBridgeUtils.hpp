#ifndef EE_X_MESSAGE_BRIDGE_UTILS_HPP
#define EE_X_MESSAGE_BRIDGE_UTILS_HPP

#include <string>

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
class MessageBridgeUtils {
public:
    static std::string call(const std::string& tag, const std::string& message);
};
} // namespace core
} // namespace ee

#endif // EE_X_MESSAGE_BRIDGE_UTILS_HPP