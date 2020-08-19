//
//  Created by eps on 12/6/19.
//

#include "ee/tenjin/private/TenjinBridge.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/PluginManager.hpp>

namespace ee {
namespace core {
template <>
std::shared_ptr<ITenjin>
PluginManager::createPluginImpl(IMessageBridge& bridge) {
    return std::make_shared<tenjin::Bridge>(bridge);
}
} // namespace core

namespace tenjin {
namespace {
// clang-format off
const std::string kPrefix = "TenjinBridge";
const auto kInitialize = kPrefix + "Initialize";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge)
    : bridge_(bridge) {
    PluginManager::addPlugin(Plugin::Tenjin);
}

Self::~Bridge() {}

void Self::destroy() {
    PluginManager::removePlugin(Plugin::Tenjin);
}

void Self::initialize(const std::string& apiKey) {
    bridge_.call(kInitialize, apiKey);
}
} // namespace tenjin
} // namespace ee
