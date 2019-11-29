//
//  jsbUtils.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/12/18.
//
#include "ee/jsb/core/jsb_utils.hpp"

#include <ee/Core.hpp>

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

#if __has_include(<base/CCDirector.h>)
#define COCOS_CREATOR_VERSION 1
#else
#define COCOS_CREATOR_VERSION 2
#endif

#include <base/CCScheduler.h>
#if COCOS_CREATOR_VERSION == 1
#include <base/CCDirector.h>
#else
#include <platform/CCApplication.h>
#endif

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, SafeInset& input) {
    auto item = nlohmann::json::object();
    item["left"] = input.left;
    item["right"] = input.right;
    item["top"] = input.top;
    item["bottom"] = input.bottom;
    auto&& obj = se::Object::createJSONObject(item.dump());
    value.setObject(obj);
}

namespace {
constexpr cocos2d::Scheduler* getScheduler(cocos2d::Scheduler* scheduler) {
    return scheduler;
}

constexpr cocos2d::Scheduler* getScheduler(const std::shared_ptr<cocos2d::Scheduler>& scheduler) {
    // Version 2.1.3
    return scheduler.get();
}

/// Gets the cocos2d-x scheduler base on the current engine version.
cocos2d::Scheduler* getScheduler() {
#if COCOS_CREATOR_VERSION == 1
    return cocos2d::Director::getInstance()->getScheduler();
#else
    auto&& scheduler = cocos2d::Application::getInstance()->getScheduler();
    return getScheduler(scheduler);
#endif
}
} // namespace

void schedule(const std::function<void(float delta)>& callback, void* target,
              float interval, const std::string& key) {
    getScheduler()->schedule(callback, target, interval, false, key);
}

void runOnCocosThread(const std::function<void()>& callback) {
    getScheduler()->performFunctionInCocosThread(callback);
};

namespace {
/// Gets the current device language code.
std::string getLanguageCode() {
    return cocos2d::Application::getInstance()->getCurrentLanguageCode();
}

// clang-format off
constexpr auto jsb_getSHA1CertificateFingerprint = &makeMethod<&getSHA1CertificateFingerprint>;
constexpr auto jsb_getVersionName                = &makeMethod<&getVersionName>;
constexpr auto jsb_getVersionCode                = &makeMethod<&getVersionCode>;
constexpr auto jsb_getLanguageCode               = &makeMethod<&getLanguageCode>;
constexpr auto jsb_isApplicationInstalled        = &makeMethod<&isApplicationInstalled>;
constexpr auto jsb_openApplication               = &makeMethod<&openApplication>;
constexpr auto jsb_sendMail                      = &makeMethod<&sendMail>;
constexpr auto jsb_isTablet                      = &makeMethod<&isTablet>;
constexpr auto jsb_testConnection                = &makeMethod<&testConnection>;
constexpr auto jsb_getDeviceId                   = &makeMethod<&getDeviceId>;
constexpr auto jsb_getSafeInset                  = &makeMethod<&getSafeInset>;
// clang-format on

SE_BIND_FUNC(jsb_getSHA1CertificateFingerprint)
SE_BIND_FUNC(jsb_getVersionName)
SE_BIND_FUNC(jsb_getVersionCode)
SE_BIND_FUNC(jsb_getLanguageCode)
SE_BIND_FUNC(jsb_isApplicationInstalled)
SE_BIND_FUNC(jsb_openApplication)
SE_BIND_FUNC(jsb_sendMail)
SE_BIND_FUNC(jsb_isTablet)
SE_BIND_FUNC(jsb_testConnection)
SE_BIND_FUNC(jsb_getDeviceId)
SE_BIND_FUNC(jsb_getSafeInset)
} // namespace

bool register_utils_manual(se::Object* global) {
    auto scope = core::getPath(global, "ee/core");

    scope->defineFunction("getSHA1CertificateFingerprint", _SE(jsb_getSHA1CertificateFingerprint));
    scope->defineFunction("getVersionName", _SE(jsb_getVersionName));
    scope->defineFunction("getVersionCode", _SE(jsb_getVersionCode));
    scope->defineFunction("getLanguageCode", _SE(jsb_getLanguageCode));
    scope->defineFunction("isApplicationInstalled", _SE(jsb_isApplicationInstalled));
    scope->defineFunction("openApplication", _SE(jsb_openApplication));
    scope->defineFunction("sendMail", _SE(jsb_sendMail));
    scope->defineFunction("isTablet", _SE(jsb_isTablet));
    scope->defineFunction("testConnection", _SE(jsb_testConnection));
    scope->defineFunction("getDeviceId", _SE(jsb_getDeviceId));
    scope->defineFunction("getSafeInset", _SE(jsb_getSafeInset));
    return true;
}
} // namespace core
} // namespace ee
