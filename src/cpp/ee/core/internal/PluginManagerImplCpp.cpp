#include "ee/core/internal/PluginManagerImplCpp.hpp"

#include "ee/core/internal/MessageBridgeCpp.hpp"

namespace ee {
namespace core {
using Self = PluginManagerImplCpp;

Self::PluginManagerImplCpp(ILogger& logger)
    : logger_(logger) {}

void Self::initialize() {
    bridge_ = std::make_shared<MessageBridgeCpp>(logger_);
}

IMessageBridge& Self::getBridge() {
    return *bridge_;
}
} // namespace core
} // namespace ee
