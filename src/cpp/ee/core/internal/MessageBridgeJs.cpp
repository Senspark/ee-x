#include "ee/core/internal/MessageBridgeJs.hpp"

#include "ee/core/Logger.hpp"
#include "ee/core/internal/MessageBridgeUtils.hpp"
#include "ee/core/Task.hpp"

namespace ee {
namespace core {
using Self = MessageBridgeJs;

Self::MessageBridgeJs() = default;
Self::~MessageBridgeJs() = default;

void Self::registerHandler(const MessageHandler& handler,
                           const std::string& tag) {
    throw std::runtime_error("Operation not supported");
}

void Self::deregisterHandler(const std::string& tag) {
    throw std::runtime_error("Operation not supported");
}

std::string Self::call(const std::string& tag, const std::string& message) {
    throw std::runtime_error("Operation not supported");
}

void Self::callCpp(const std::string& tag, const std::string& message) {
    // FIXME.
}

Task<std::string> Self::callAsync(const std::string& tag,
                                  const std::string& message) {
    throw std::runtime_error("Operation not supported");
}
} // namespace core
} // namespace ee
