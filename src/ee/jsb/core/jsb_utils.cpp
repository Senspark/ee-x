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
// TODO:
// REDUDE ARGUMENT DECLARATIONS
// https://stackoverflow.com/questions/37426283/get-the-type-of-a-function-parameter-with-boosthana

namespace core {
void runOnCocosThread(const std::function<void()>& callback) {
#if COCOS_CREATOR_VERSION == 1
    cocos2d::Director::getInstance()
        ->getScheduler()
        ->performFunctionInCocosThread(callback);
#else
    cocos2d::Application::getInstance()
        ->getScheduler()
        ->performFunctionInCocosThread(callback);
#endif
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
constexpr auto jsb_getDeviceId                   = &makeMethod<&getDeviceId>;
// clang-format on

SE_BIND_FUNC(jsb_getSHA1CertificateFingerprint)
SE_BIND_FUNC(jsb_getVersionName)
SE_BIND_FUNC(jsb_getVersionCode)
SE_BIND_FUNC(jsb_getLanguageCode)
SE_BIND_FUNC(jsb_isApplicationInstalled)
SE_BIND_FUNC(jsb_openApplication)
SE_BIND_FUNC(jsb_sendMail)
SE_BIND_FUNC(jsb_isTablet)
SE_BIND_FUNC(jsb_getDeviceId)
} // namespace

bool register_utils_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* coreObj = nullptr;
    getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    getOrCreatePlainObject_r("core", eeObj, &coreObj);

    coreObj->defineFunction("getSHA1CertificateFingerprint",
                            _SE(jsb_getSHA1CertificateFingerprint));
    coreObj->defineFunction("getVersionName", _SE(jsb_getVersionName));
    coreObj->defineFunction("getVersionCode", _SE(jsb_getVersionCode));
    coreObj->defineFunction("getLanguageCode", _SE(jsb_getLanguageCode));
    coreObj->defineFunction("isApplicationInstalled",
                            _SE(jsb_isApplicationInstalled));
    coreObj->defineFunction("openApplication", _SE(jsb_openApplication));
    coreObj->defineFunction("sendMail", _SE(jsb_sendMail));
    coreObj->defineFunction("isTablet", _SE(jsb_isTablet));
    coreObj->defineFunction("getDeviceId", _SE(jsb_getDeviceId));
    return true;
}
} // namespace core
} // namespace ee
