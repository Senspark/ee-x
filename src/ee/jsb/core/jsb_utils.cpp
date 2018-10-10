//
//  jsbUtils.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/12/18.
//

#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_core_common.hpp"
#include "jsb_utils.hpp"

#include <ee/Core.hpp>

namespace ee {
int getIntergerUserDefault(std::string key) {
    return cocos2d::UserDefault::getInstance()->getIntegerForKey(key.c_str());
};

namespace core {
constexpr auto jsb_getSHA1CertificateFingerprint =
    &jsb_static_get<std::string, &ee::getSHA1CertificateFingerprint>;
constexpr auto jsb_getVersionName =
    &jsb_static_get<std::string, &ee::getVersionName>;
constexpr auto jsb_getVersionCode =
    &jsb_static_get<std::string, &ee::getVersionCode>;
constexpr auto jsb_isApplicationInstalled =
    &jsb_static_get<bool, &ee::isApplicationInstalled, std::string>;
constexpr auto jsb_openApplication =
    &jsb_static_call<&ee::openApplication, std::string>;
constexpr auto jsb_sendMail =
    &jsb_static_call<&ee::sendMail, std::string, std::string, std::string>;
constexpr auto jsb_isTablet = &jsb_static_get<bool, &ee::isTablet>;
constexpr auto jsb_getIntergerUserDefault =
    &jsb_static_get<int, &ee::getIntergerUserDefault, std::string>;
// constexpr auto jsb_getDeviceId = &jsb_static_get<std::string,
// &ee::getDeviceId>;

SE_BIND_FUNC(jsb_getSHA1CertificateFingerprint)
SE_BIND_FUNC(jsb_getVersionName)
SE_BIND_FUNC(jsb_getVersionCode)
SE_BIND_FUNC(jsb_isApplicationInstalled)
SE_BIND_FUNC(jsb_openApplication)
SE_BIND_FUNC(jsb_sendMail)
SE_BIND_FUNC(jsb_isTablet)
SE_BIND_FUNC(jsb_getIntergerUserDefault)
// SE_BIND_FUNC(jsb_getDeviceId)

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
    //    __coreObj->defineFunction("getDeviceId", _SE(jsb_getDeviceId));
    return true;
}
} // namespace core
} // namespace ee
