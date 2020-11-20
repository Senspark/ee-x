#include "ee/ad_colony/private/AdColonyBridge.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/SwitchToLibraryThread.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>

namespace ee {
namespace core {
template <>
std::shared_ptr<IAdColony>
PluginManager::createPluginImpl(IMessageBridge& bridge) {
    if (not addPlugin(Plugin::AdColony)) {
        return nullptr;
    }
    return std::make_shared<ad_colony::Bridge>(bridge);
}
} // namespace core

namespace ad_colony {
namespace {
const std::string kPrefix = "AdColonyBridge";
const std::string kInitialize = kPrefix + "Initialize";
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge)
    : bridge_(bridge)
    , logger_(Logger::getSystemLogger()) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
}

Self::~Bridge() = default;

void Self::destroy() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    PluginManager::removePlugin(Plugin::AdMob);
}

Task<bool> Self::initialize(const std::string& appId) {
    auto response = co_await bridge_.callAsync(kInitialize, appId);
    co_await SwitchToLibraryThread();
    co_return core::toBool(response);
}
} // namespace ad_colony
} // namespace ee
