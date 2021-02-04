#include "ee/firebase_crashlytics/private/FirebaseCrashlyticsBridge.hpp"

#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>

namespace ee {
namespace firebase {
namespace crashlytics {
namespace {
const std::string kPrefix = "FirebaseCrashlyticsBridge";
const auto kInitialize = kPrefix + "Initialize";
const auto kLog = kPrefix + "Log";
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge, ILogger& logger,
             const Destroyer& destroyer)
    : bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer) {}

Self::~Bridge() = default;

void Self::destroy() {
    destroyer_();
}

Task<bool> Self::initialize() {
    auto response = co_await bridge_.callAsync(kInitialize);
    return core::toBool(response);
}

void Self::log(const std::string& message) {
    bridge_.call(kLog, message);
}
} // namespace crashlytics
} // namespace firebase
} // namespace ee
