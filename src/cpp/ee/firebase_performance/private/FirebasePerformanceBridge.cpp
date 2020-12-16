//
//  FirebasePerformance.cpp
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/8/19.
//

#include "ee/firebase_performance/private/FirebasePerformanceBridge.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Utils.hpp>

#include "ee/firebase_performance/private/FirebasePerformanceTrace.hpp"

namespace ee {
namespace core {
template <>
std::shared_ptr<IFirebasePerformance>
PluginManager::createPluginImpl(IMessageBridge& bridge) {
    if (not addPlugin(Plugin::FirebasePerformance)) {
        return nullptr;
    }
    return std::make_shared<firebase::performance::Bridge>(bridge);
}
} // namespace core

namespace firebase {
namespace performance {
namespace {
// clang-format off
const std::string kPrefix = "FirebasePerformanceBridge";
const auto kSetDataCollectionEnabled = kPrefix + "SetDataCollectionEnabled";
const auto kIsDataCollectionEnabled  = kPrefix + "IsDataCollectionEnabled";
const auto kNewTrace                 = kPrefix + "NewTrace";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge)
    : bridge_(bridge) {}

Self::~Bridge() = default;

void Self::destroy() {
    PluginManager::removePlugin(Plugin::FirebasePerformance);
}

void Self::setDataCollectionEnabled(bool enabled) {
    bridge_.call(kSetDataCollectionEnabled, core::toString(enabled));
}

bool Self::isDataCollectionEnabled() {
    auto response = bridge_.call(kIsDataCollectionEnabled);
    return core::toBool(response);
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
