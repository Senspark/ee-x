#include "ee/core/internal/PluginManagerImplJs.hpp"

#include "ee/core/internal/MessageBridgeJs.hpp"

namespace ee {
namespace core {
using Self = PluginManagerImplJs;

void Self::initialize() {
    bridge_ = std::make_shared<MessageBridgeJs>();
}

IMessageBridge& Self::getBridge() {
    return *bridge_;
}
} // namespace core
} // namespace ee
