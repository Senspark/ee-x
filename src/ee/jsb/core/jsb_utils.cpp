//
//  jsbUtils.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/12/18.
//

#include <ee/Core.hpp>

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/core/jsb_utils.hpp"

namespace ee {
int getIntergerUserDefault(std::string key) {
    return cocos2d::UserDefault::getInstance()->getIntegerForKey(key.c_str());
};

// TODO:
// REDUDE ARGUMENT DECLARATIONS
// https://stackoverflow.com/questions/37426283/get-the-type-of-a-function-parameter-with-boosthana

namespace core {
constexpr auto jsb_getSHA1CertificateFingerprint =
    &jsb_static_get<const std::string&, &getSHA1CertificateFingerprint>;
constexpr auto jsb_getVersionName =
    &jsb_static_get<const std::string&, &getVersionName>;
constexpr auto jsb_getVersionCode =
    &jsb_static_get<const std::string&, &getVersionCode>;
constexpr auto jsb_isApplicationInstalled =
    &jsb_static_get<bool, &isApplicationInstalled, const std::string&>;
constexpr auto jsb_openApplication =
    &jsb_static_call<&openApplication, const std::string&>;
constexpr auto jsb_sendMail =
    &jsb_static_call<&sendMail, const std::string&, const std::string&,
                     const std::string&>;
constexpr auto jsb_isTablet = &jsb_static_get<bool, &isTablet>;
constexpr auto jsb_getIntergerUserDefault =
    &jsb_static_get<int, &getIntergerUserDefault, const std::string&>;
constexpr auto jsb_getDeviceId =
    &jsb_static_set_callback<&getDeviceId, const std::string&>;

SE_BIND_FUNC(jsb_getSHA1CertificateFingerprint)
SE_BIND_FUNC(jsb_getVersionName)
SE_BIND_FUNC(jsb_getVersionCode)
SE_BIND_FUNC(jsb_isApplicationInstalled)
SE_BIND_FUNC(jsb_openApplication)
SE_BIND_FUNC(jsb_sendMail)
SE_BIND_FUNC(jsb_isTablet)
SE_BIND_FUNC(jsb_getIntergerUserDefault)
SE_BIND_FUNC(jsb_getDeviceId)

bool register_utils_manual(se::Object* globalObj) {
    if (__eeObj == nullptr || __coreObj == nullptr) {
        getOrCreatePlainObject_r("ee", globalObj, &__eeObj);
        getOrCreatePlainObject_r("core", __eeObj, &__coreObj);
    }

    __coreObj->defineFunction("getSHA1CertificateFingerprint",
                              _SE(jsb_getSHA1CertificateFingerprint));
    __coreObj->defineFunction("getVersionName", _SE(jsb_getVersionName));
    __coreObj->defineFunction("getVersionCode", _SE(jsb_getVersionCode));
    __coreObj->defineFunction("isApplicationInstalled",
                              _SE(jsb_isApplicationInstalled));
    __coreObj->defineFunction("openApplication", _SE(jsb_openApplication));
    __coreObj->defineFunction("sendMail", _SE(jsb_sendMail));
    __coreObj->defineFunction("isTablet", _SE(jsb_isTablet));
    __coreObj->defineFunction("getIntergerUserDefault",
                              _SE(jsb_getIntergerUserDefault));
    __coreObj->defineFunction("getDeviceId", _SE(jsb_getDeviceId));
    return true;
}
} // namespace core
} // namespace ee
