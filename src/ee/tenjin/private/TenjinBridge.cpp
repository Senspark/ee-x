//
//  Created by eps on 12/6/19.
//

#include "ee/tenjin/private/TenjinBridge.hpp"

#include <ee/core/PluginManager.hpp>
#include <ee/core/internal/MessageBridge.hpp>

namespace ee {
namespace core {
template <>
std::unique_ptr<ITenjin> PluginManager::createPlugin() {
    return std::make_unique<tenjin::Bridge>();
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

Self::Bridge()
    : bridge_(MessageBridge::getInstance()) {
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
