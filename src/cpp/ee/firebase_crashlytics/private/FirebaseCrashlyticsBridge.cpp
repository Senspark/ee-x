#include "ee/firebase_crashlytics/private/FirebaseCrashlyticsBridge.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/PluginManager.hpp>

namespace ee {
namespace core {
template <>
std::shared_ptr<IFirebaseCrashlytics>
PluginManager::createPluginImpl(IMessageBridge& bridge) {
    addPlugin(Plugin::FirebaseCrashlytics);
    return std::make_shared<firebase::crashlytics::Bridge>(bridge);
}
} // namespace core

namespace firebase {
namespace crashlytics {
namespace {
const std::string kPrefix = "FirebaseCrashlyticsBridge";
const auto kLog = kPrefix + "Log";
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge)
    : bridge_(bridge) {}

Self::~Bridge() = default;

void Self::destroy() {
    PluginManager::removePlugin(Plugin::FirebaseCrashlytics);
}

void Self::log(const std::string& message) {
    bridge_.call(kLog, message);
}
} // namespace crashlytics
} // namespace firebase
} // namespace ee
