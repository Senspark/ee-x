//
//  GoogleAnalyticsTest.cpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#include "GoogleAnalyticsTester.hpp"

#include <ee/GoogleAnalytics.hpp>

namespace eetest {
namespace google {
using Self = Tester;

void Self::initialize() {
    plugin_ = std::make_shared<ee::GoogleAnalytics>();
    plugin_->setDispatchInterval(10);
    plugin_->setDryRun(false);
    plugin_->setOptOut(false);
    plugin_->setTrackUncaughtException(true);
    plugin_->doTests();
}

void Self::destroy() {
    plugin_->destroy();
}

void Self::start() {
    auto tracker = plugin_->createTracker("TEST");
    tracker->setAllowIDFACollection(true);
    tracker->setParameter("key", "value");
    tracker->setScreenName("screen_name");
    tracker->send({
        {"key1", "value1"},
        {"key2", "value2"},
    });
}

void Self::stop() {}
} // namespace google
} // namespace eetest
