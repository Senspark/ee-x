//
//  FirebasePerformance.cpp
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/8/19.
//

#include "ee/firebase_performance/private/FirebasePerformanceBridge.hpp"

#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Utils.hpp>

#include "ee/firebase_performance/private/FirebasePerformanceTrace.hpp"

namespace ee {
namespace firebase {
namespace performance {
namespace {
// clang-format off
const std::string kPrefix = "FirebasePerformanceBridge";
const auto kIsDataCollectionEnabled  = kPrefix + "IsDataCollectionEnabled";
const auto kSetDataCollectionEnabled = kPrefix + "SetDataCollectionEnabled";
const auto kNewTrace                 = kPrefix + "NewTrace";
// clang-format on
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

bool Self::isDataCollectionEnabled() {
    auto response = bridge_.call(kIsDataCollectionEnabled);
    return core::toBool(response);
}

void Self::setDataCollectionEnabled(bool enabled) {
    bridge_.call(kSetDataCollectionEnabled, core::toString(enabled));
}

std::shared_ptr<ITrace> Self::newTrace(const std::string& name) {
    auto response = bridge_.call(kNewTrace, name);
    if (not core::toBool(response)) {
        return nullptr;
    }
    return std::make_shared<Trace>(bridge_, this, name);
}
} // namespace performance
} // namespace firebase
} // namespace ee
