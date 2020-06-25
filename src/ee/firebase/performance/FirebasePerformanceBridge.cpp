//
//  FirebasePerformance.cpp
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/8/19.
//

#include "ee/firebase/performance/FirebasePerformanceBridge.hpp"

#include <ee/core/PluginManager.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

#include "ee/firebase/performance/FirebasePerformanceTrace.hpp"

namespace ee {
namespace firebase {
namespace performance {
using Self = Bridge;

namespace {
// clang-format off
const std::string kPrefix = "FirebasePerformanceBridge";
const auto kSetDataCollectionEnabled = kPrefix + "SetDataCollectionEnabled";
const auto kIsDataCollectionEnabled  = kPrefix + "IsDataCollectionEnabled";
const auto kStartTrace               = kPrefix + "StartTrace";
const auto kNewTrace                 = kPrefix + "NewTrace";
// clang-format on
} // namespace

Self::Bridge() {
    PluginManager::addPlugin(Plugin::FirebasePerformance);
}

void Self::destroy() {
    PluginManager::removePlugin(Plugin::FirebasePerformance);
}

void Self::setDataCollectionEnabled(bool enabled) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(kSetDataCollectionEnabled, ee::core::toString(enabled));
}

bool Self::isDataCollectionEnabled() {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kIsDataCollectionEnabled);
    return ee::core::toBool(response);
}

std::shared_ptr<Trace> Self::startTrace(const std::string& name) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kStartTrace, name);
    if (not ee::core::toBool(response)) {
        return nullptr;
    }
    return std::shared_ptr<Trace>(new Trace(this, name));
}

std::shared_ptr<Trace> Self::newTrace(const std::string& name) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kNewTrace, name);
    if (not ee::core::toBool(response)) {
        return nullptr;
    }
    return std::shared_ptr<Trace>(new Trace(this, name));
}
} // namespace performance
} // namespace firebase
} // namespace ee
