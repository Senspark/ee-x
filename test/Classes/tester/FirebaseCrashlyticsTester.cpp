//
//  FirebaseCrashlyticsTester.cpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#include "FirebaseCrashlyticsTester.hpp"

#include <ee/Firebase.hpp>

namespace eetest {
namespace firebase {
namespace crashlytics {
using Self = Tester;

void Self::initialize() {
    plugin_ = std::make_shared<ee::FirebaseCrashlytics>();
}

void Self::destroy() {
    plugin_->destroy();
}

void Self::start() {
    plugin_->log("TEST");
}

void Self::stop() {}
} // namespace crashlytics
} // namespace firebase
} // namespace eetest
