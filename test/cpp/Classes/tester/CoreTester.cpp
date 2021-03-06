//
//  CoreTester.cpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#include "CoreTester.hpp"

#include <ee/Cpp.hpp>

#include "Utils.hpp"

namespace eetest {
namespace core {
using Self = Tester;

void Self::initialize() {}

void Self::destroy() {}

void Self::start() {
    getLogger().info("getApplicationId: %s", ee::getApplicationId().c_str());
    getLogger().info("getApplicationName: %s",
                     ee::getApplicationName().c_str());
    getLogger().info("getDensity: %f", ee::getDensity());
    ee::noAwait([]() -> ee::Task<> {
        auto result = co_await ee::getDeviceId();
        getLogger().info("getDeviceId: %s", result.c_str());
    });
    auto inset = ee::getSafeInset();
    getLogger().info("getSafeInset: %d %d %d %d", inset.right, inset.top,
                     inset.left, inset.bottom);
    getLogger().info("getSHA1CertificateFingerprint: %s",
                     ee::getSHA1CertificateFingerprint().c_str());
    getLogger().info("isInstantApp: %d", ee::isInstantApp());
    getLogger().info("getVersionCode: %s", ee::getVersionCode().c_str());
    getLogger().info("getVersionName: %s", ee::getVersionCode().c_str());
    getLogger().info("isApplicationInstalled: %d",
                     ee::isApplicationInstalled("com.ee.test"));
    getLogger().info("isTablet: %d", ee::isTablet());
    ee::noAwait([]() -> ee::Task<> {
        auto result = co_await ee::testConnection("www.google.com", 1.0f);
        getLogger().info("testConnection: %d", result);
    });
    ee::noAwait([]() -> ee::Task<> {
        auto result = co_await ee::getInstallReferrer();
        getLogger().info("getInstallReferrer: raw = %s "
                         "utm_source = %s "
                         "utm_medium = %s "
                         "utm_term = %s "
                         "utm_content = %s "
                         "utm_campaign = %s",
                         result.raw.c_str(),        //
                         result.utm_source.c_str(), //
                         result.utm_medium.c_str(), //
                         result.utm_term.c_str(),   //
                         result.utm_content.c_str(),
                         result.utm_campaign.c_str());
    });
}

void Self::stop() {}
} // namespace core
} // namespace eetest
