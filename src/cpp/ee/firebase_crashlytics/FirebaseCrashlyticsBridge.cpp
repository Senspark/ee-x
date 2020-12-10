#include "ee/firebase/crashlytics/FirebaseCrashlyticsBridge.hpp"

#include <ee/core/PluginManager.hpp>
#include <ee/core/internal/MessageBridge.hpp>

namespace ee {
namespace firebase {
namespace crashlytics {
using Self = Bridge;

namespace {
const std::string kPrefix = "FirebaseCrashlyticsBridge";
const auto kLog = kPrefix + "Log";
} // namespace

Self::Bridge()
    : bridge_(MessageBridge::getInstance()) {
    PluginManager::addPlugin(Plugin::FirebaseCrashlytics);
}

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
