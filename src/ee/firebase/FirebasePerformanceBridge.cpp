//
//  FirebasePerformance.cpp
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/8/19.
//

#include "ee/firebase/FirebasePerformanceBridge.hpp"

#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"
#include "ee/firebase/FirebasePerformanceTrace.hpp"

namespace ee {
namespace firebase {
using Self = Performance;

namespace {
// clang-format off
constexpr auto k__setDataCollectionEnabled  = "FirebasePerformance_setDataCollectionEnabled";
constexpr auto k__isDataCollectionEnabled   = "FirebasePerformance_isDataCollectionEnabled";
constexpr auto k__startTrace                = "FirebasePerformance_startTrace";
constexpr auto k__newTrace                  = "FirebasePerformance_newTrace";
// clang-format on
} // namespace

void Self::setDataCollectionEnabled(bool enabled) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__setDataCollectionEnabled, core::toString(enabled));
}

bool Self::isDataCollectionEnabled() {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__isDataCollectionEnabled);
    return core::toBool(response);
}

std::shared_ptr<PerformanceTrace> Self::startTrace(const std::string& name) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__startTrace, name);
    if (not core::toBool(response)) {
        return nullptr;
    }
    return std::shared_ptr<PerformanceTrace>(new PerformanceTrace(this, name));
}

std::shared_ptr<PerformanceTrace> Self::newTrace(const std::string& name) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__newTrace, name);
    if (not core::toBool(response)) {
        return nullptr;
    }
    return std::shared_ptr<PerformanceTrace>(new PerformanceTrace(this, name));
}
} // namespace firebase
} // namespace ee
