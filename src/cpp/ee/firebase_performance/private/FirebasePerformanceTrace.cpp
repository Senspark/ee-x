//
//  FirebasePerformanceTrace.cpp
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/8/19.
//

#include "ee/firebase_performance/private/FirebasePerformanceTrace.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace firebase {
namespace performance {
namespace {
// clang-format off
constexpr auto k__key       = "key";
constexpr auto k__value     = "value";
// clang-format on
} // namespace

namespace {
std::string k__start(const std::string& name) {
    return "FirebasePerformance_start_" + name;
}

std::string k__stop(const std::string& name) {
    return "FirebasePerformance_stop_" + name;
}

std::string k__incrementMetric(const std::string& name) {
    return "FirebasePerformance_incrementMetric_" + name;
}

std::string k__getLongMetric(const std::string& name) {
    return "FirebasePerformance_getLongMetric_" + name;
}

std::string k__putMetric(const std::string& name) {
    return "FirebasePerformance_putMetric_" + name;
}
} // namespace

using Self = Trace;

Self::Trace(IMessageBridge& bridge, Bridge* plugin, const std::string& name)
    : bridge_(bridge)
    , plugin_(plugin)
    , name_(name) {}

void Self::start() {
    bridge_.call(k__start(name_));
}

void Self::stop() {
    bridge_.call(k__stop(name_));
}

void Self::putMetric(const std::string& metricName, std::int64_t value) {
    nlohmann::json json;
    json[k__key] = metricName;
    json[k__value] = value;
    bridge_.call(k__putMetric(name_), json.dump());
}

void Self::incrementMetric(const std::string& metricName,
                           std::int64_t incrementBy) {
    nlohmann::json json;
    json[k__key] = metricName;
    json[k__value] = incrementBy;
    bridge_.call(k__incrementMetric(name_), json.dump());
}

std::int64_t Self::getLongMetric(const std::string& metricName) {
    auto response = bridge_.call(k__getLongMetric(name_), metricName);
    return std::stoll(response);
}
} // namespace performance
} // namespace firebase
} // namespace ee
