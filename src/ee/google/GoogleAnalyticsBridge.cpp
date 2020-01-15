//
//  GoogleAnalytics.cpp
//  ee_x
//
//  Created by Zinge on 10/24/17.
//
//

#include "ee/google/GoogleAnalyticsBridge.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/core/MessageBridge.hpp>
#include <ee/core/Utils.hpp>

#include "ee/google/GoogleAnalyticsEventBuilder.hpp"
#include "ee/google/GoogleAnalyticsExceptionBuilder.hpp"
#include "ee/google/GoogleAnalyticsProduct.hpp"
#include "ee/google/GoogleAnalyticsProductAction.hpp"
#include "ee/google/GoogleAnalyticsScreenViewBuilder.hpp"
#include "ee/google/GoogleAnalyticsSocialBuilder.hpp"
#include "ee/google/GoogleAnalyticsTimingBuilder.hpp"
#include "ee/google/GoogleAnalyticsTracker.hpp"

namespace ee {
namespace google {
using Self = Analytics;

namespace {
// clang-format off
constexpr auto k__setDispatchInterval          = "GoogleAnalytics_setDispatchInterval";
constexpr auto k__setDryRun                    = "GoogleAnalytics_setDryRun";
constexpr auto k__setOptOut                    = "GoogleAnalytics_setOptOut";
constexpr auto k__setTrackUncaughtException    = "GoogleAnalytics_setTrackUncaughtException";
constexpr auto k__dispatch                     = "GoogleAnalytics_dispatch";
constexpr auto k__createTracker                = "GoogleAnalytics_createTracker";
constexpr auto k__destroyTracker               = "GoogleAnalytics_destroyTracker";
    
constexpr auto k__testTrackEvent               = "GoogleAnalytics_testTrackEvent";
constexpr auto k__testTrackException           = "GoogleAnalytics_testTrackException";
constexpr auto k__testTrackScreenView          = "GoogleAnalytics_testTrackScreenView";
constexpr auto k__testTrackSocial              = "GoogleAnalytics_testTrackSocial";
constexpr auto k__testTrackTiming              = "GoogleAnalytics_testTrackTiming";
constexpr auto k__testCustomDimensionAndMetric = "GoogleAnalytics_testCustomDimensionAndMetric";
constexpr auto k__testTrackEcommerceAction     = "GoogleAnalytics_testTrackEcommerceAction";
constexpr auto k__testTrackEcommerceImpression = "GoogleAnalytics_testTrackEcommerceImpression";
// clang-format on
} // namespace

Self::Analytics() {}

Self::~Analytics() {}

void Self::setDispatchInterval(int seconds) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__setDispatchInterval, std::to_string(seconds));
}

void Self::setDryRun(bool enabled) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__setDryRun, core::toString(enabled));
}

void Self::setOptOut(bool enabled) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__setOptOut, core::toString(enabled));
}

void Self::setTrackUncaughtException(bool enabled) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__setTrackUncaughtException, core::toString(enabled));
}

void Self::dispatch() {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__dispatch);
}

std::shared_ptr<AnalyticsTracker>
Self::createTracker(const std::string& trackingId) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__createTracker, trackingId);
    if (not core::toBool(response)) {
        return nullptr;
    }
    return std::shared_ptr<AnalyticsTracker>(
        new AnalyticsTracker(this, trackingId));
}

bool Self::destroyTracker(const std::string& trackingId) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__destroyTracker, trackingId);
    return core::toBool(response);
}

int Self::doTests() {
    int passed = 0;
    passed += testTrackEvent();
    passed += testTrackException();
    passed += testTrackScreenView();
    passed += testTrackSocial();
    passed += testTrackTiming();
    passed += testCustomDimensionAndMetric();
    passed += testEcommerceAction();
    passed += testEcommerceImpression();

    constexpr int total = 8;
    return total - passed;
}

bool Self::testTrackEvent() {
    auto dict = EventBuilder()
                    .setCategory("category")
                    .setAction("action")
                    .setLabel("label")
                    .setValue(1)
                    .build();
    nlohmann::json json = dict;
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__testTrackEvent, json.dump());
    return core::toBool(response);
}

bool Self::testTrackException() {
    auto dict =
        ExceptionBuilder().setDescription("description").setFatal(true).build();
    nlohmann::json json = dict;
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__testTrackException, json.dump());
    return core::toBool(response);
}

bool Self::testTrackScreenView() {
    auto dict = ScreenViewBuilder().build();
    nlohmann::json json = dict;
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__testTrackScreenView, json.dump());
    return core::toBool(response);
}

bool Self::testTrackSocial() {
    auto dict = SocialBuilder()
                    .setNetwork("network")
                    .setAction("action")
                    .setTarget("target")
                    .build();
    nlohmann::json json = dict;
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__testTrackSocial, json.dump());
    return core::toBool(response);
}

bool Self::testTrackTiming() {
    auto dict = TimingBuilder()
                    .setCategory("category")
                    .setVariable("variable")
                    .setLabel("label")
                    .setValue(1)
                    .build();
    nlohmann::json json = dict;
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__testTrackTiming, json.dump());
    return core::toBool(response);
}

bool Self::testCustomDimensionAndMetric() {
    auto dict = ScreenViewBuilder()
                    .setCustomMetric(1, 1)
                    .setCustomMetric(2, 2)
                    .setCustomMetric(5, 5.5f)
                    .setCustomDimension(1, "dimension_1")
                    .setCustomDimension(2, "dimension_2")
                    .build();
    nlohmann::json json = dict;
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__testCustomDimensionAndMetric, json.dump());
    return core::toBool(response);
}

bool Self::testEcommerceAction() {
    auto product0 = Product()
                        .setCategory("category0")
                        .setId("id0")
                        .setName("name0")
                        .setPrice(1.5f);
    auto product1 = Product()
                        .setCategory("category1")
                        .setId("id1")
                        .setName("name1")
                        .setPrice(2);
    auto action = ProductAction(ProductAction::ActionPurchase)
                      .setProductActionList("actionList")
                      .setProductListSource("listSource")
                      .setTransactionId("transactionId")
                      .setTransactionRevenue(2.0f);
    auto dict = ScreenViewBuilder()
                    .addProduct(product0)
                    .addProduct(product1)
                    .setProductAction(action)
                    .build();
    nlohmann::json json = dict;
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__testTrackEcommerceAction, json.dump());
    return core::toBool(response);
}

bool Self::testEcommerceImpression() {
    auto product0 = Product()
                        .setCategory("category0")
                        .setId("id0")
                        .setName("name0")
                        .setPrice(1.5f);
    auto product1 = Product()
                        .setCategory("category1")
                        .setId("id1")
                        .setName("name1")
                        .setPrice(2.5f);
    auto product2 = Product()
                        .setCategory("category2")
                        .setId("id2")
                        .setName("name2")
                        .setPrice(3.0f);
    auto product3 = Product()
                        .setCategory("category3")
                        .setId("id3")
                        .setName("name3")
                        .setPrice(4);
    auto dict = ScreenViewBuilder()
                    .addImpression(product0, "impressionList0")
                    .addImpression(product1, "impressionList0")
                    .addImpression(product2, "impressionList1")
                    .addImpression(product3, "impressionList1")
                    .build();
    nlohmann::json json = dict;
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__testTrackEcommerceImpression, json.dump());
    return core::toBool(response);
}
} // namespace google
} // namespace ee
