//
//  GoogleAnalytics.cpp
//  ee_x
//
//  Created by Zinge on 10/24/17.
//
//

#include "ee/google/GoogleAnalyticsBridge.hpp"

#include <ee/core/PluginManager.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>
#include <ee/nlohmann/json.hpp>

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
const std::string kPrefix = "GoogleAnalyticsBridge";
const auto kSetDispatchInterval          = kPrefix + "SetDispatchInterval";
const auto kSetDryRun                    = kPrefix + "SetDryRun";
const auto kSetOptOut                    = kPrefix + "SetOptOut";
const auto kSetTrackUncaughtException    = kPrefix + "SetTrackUncaughtException";
const auto kDispatch                     = kPrefix + "Dispatch";
const auto kCreateTracker                = kPrefix + "CreateTracker";
const auto kDestroyTracker               = kPrefix + "DestroyTracker";
    
const auto kTestTrackEvent               = kPrefix + "TestTrackEvent";
const auto kTestTrackException           = kPrefix + "TestTrackException";
const auto kTestTrackScreenView          = kPrefix + "TestTrackScreenView";
const auto kTestTrackSocial              = kPrefix + "TestTrackSocial";
const auto kTestTrackTiming              = kPrefix + "TestTrackTiming";
const auto kTestCustomDimensionAndMetric = kPrefix + "TestCustomDimensionAndMetric";
const auto kTestTrackEcommerceAction     = kPrefix + "TestTrackEcommerceAction";
const auto kTestTrackEcommerceImpression = kPrefix + "TestTrackEcommerceImpression";
// clang-format on
} // namespace

Self::Analytics() {
    PluginManager::addPlugin(Plugin::GoogleAnalytics);
}

Self::~Analytics() = default;

void Self::destroy() {
    PluginManager::removePlugin(Plugin::GoogleAnalytics);
}

void Self::setDispatchInterval(int seconds) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(kSetDispatchInterval, std::to_string(seconds));
}

void Self::setDryRun(bool enabled) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(kSetDryRun, core::toString(enabled));
}

void Self::setOptOut(bool enabled) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(kSetOptOut, core::toString(enabled));
}

void Self::setTrackUncaughtException(bool enabled) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(kSetTrackUncaughtException, core::toString(enabled));
}

void Self::dispatch() {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(kDispatch);
}

std::shared_ptr<AnalyticsTracker>
Self::createTracker(const std::string& trackingId) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kCreateTracker, trackingId);
    if (not core::toBool(response)) {
        return nullptr;
    }
    return std::shared_ptr<AnalyticsTracker>(
        new AnalyticsTracker(this, trackingId));
}

bool Self::destroyTracker(const std::string& trackingId) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kDestroyTracker, trackingId);
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
    auto response = bridge.call(kTestTrackEvent, json.dump());
    return core::toBool(response);
}

bool Self::testTrackException() {
    auto dict =
        ExceptionBuilder().setDescription("description").setFatal(true).build();
    nlohmann::json json = dict;
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kTestTrackException, json.dump());
    return core::toBool(response);
}

bool Self::testTrackScreenView() {
    auto dict = ScreenViewBuilder().build();
    nlohmann::json json = dict;
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kTestTrackScreenView, json.dump());
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
    auto response = bridge.call(kTestTrackSocial, json.dump());
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
    auto response = bridge.call(kTestTrackTiming, json.dump());
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
    auto response = bridge.call(kTestCustomDimensionAndMetric, json.dump());
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
    auto response = bridge.call(kTestTrackEcommerceAction, json.dump());
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
    auto response = bridge.call(kTestTrackEcommerceImpression, json.dump());
    return core::toBool(response);
}
} // namespace google
} // namespace ee
