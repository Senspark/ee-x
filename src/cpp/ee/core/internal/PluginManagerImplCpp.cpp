#include "ee/core/internal/PluginManagerImplCpp.hpp"

#include "ee/core/internal/MessageBridgeCpp.hpp"

namespace ee {
namespace core {
using Self = PluginManagerImplCpp;

void Self::initialize() {
    bridge_ = std::make_shared<MessageBridgeCpp>();
}

IMessageBridge& Self::getBridge() {
    return *bridge_;
}
} // namespace core
} // namespace ee
