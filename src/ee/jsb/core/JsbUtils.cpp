//
//  jsbUtils.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/12/18.
//

#include "ee/jsb/core/JsbUtils.hpp"

#include <platform/CCApplication.h>

#include <ee/nlohmann/json.hpp>

#include <ee/Core.hpp>

#include "ee/jsb/core/Templates.hpp"
#include "ee/jsb/core/Utils.hpp"

namespace ee {
namespace core {
void set_value_impl(se::Value& value, SafeInset& input) {
    auto item = nlohmann::json::object();
    item["left"] = input.left;
    item["right"] = input.right;
    item["top"] = input.top;
    item["bottom"] = input.bottom;
    auto&& obj = se::Object::createJSONObject(item.dump());
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, SafeInset& input) {
    set_value_impl(value, input);
}

template <>
void set_value(se::Value& value, SafeInset input) {
    set_value_impl(value, input);
}

namespace {
/// Gets the current device language code.
std::string getLanguageCode() {
    return cocos2d::Application::getInstance()->getCurrentLanguageCode();
}

void getSafeInsetAsync(const std::function<void(const SafeInset& inset)>& callback) {
    runOnUiThread([callback] {
        auto inset = getSafeInset();
        runOnCocosThread([callback, inset] {
            callback(inset);
        });
    });
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
constexpr auto jsb_getSafeInsetAsync             = &makeMethod<&getSafeInsetAsync>;
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
SE_BIND_FUNC(jsb_getSafeInsetAsync)
} // namespace

bool registerJsbUtils(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    scope->defineFunction("getDeviceId", _SE(jsb_getDeviceId));
    scope->defineFunction("getSHA1CertificateFingerprint", _SE(jsb_getSHA1CertificateFingerprint));
    scope->defineFunction("getVersionCode", _SE(jsb_getVersionCode));
    scope->defineFunction("getVersionName", _SE(jsb_getVersionName));
    scope->defineFunction("getLanguageCode", _SE(jsb_getLanguageCode));
    scope->defineFunction("isApplicationInstalled", _SE(jsb_isApplicationInstalled));
    scope->defineFunction("isTablet", _SE(jsb_isTablet));
    scope->defineFunction("openApplication", _SE(jsb_openApplication));
    scope->defineFunction("sendMail", _SE(jsb_sendMail));
    scope->defineFunction("testConnection", _SE(jsb_testConnection));
    scope->defineFunction("getSafeInset", _SE(jsb_getSafeInset));
    scope->defineFunction("getSafeInsetAsync", _SE(jsb_getSafeInsetAsync));
    return true;
}
} // namespace core
} // namespace ee
