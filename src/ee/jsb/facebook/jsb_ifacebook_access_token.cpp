//
//  jsb_ifacebook_access_token.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/jsb/facebook/jsb_ifacebook_access_token.hpp"

#include "ee/facebook/IFacebookAccessToken.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace facebook {
se::Class* __jsb_AccessToken_class = nullptr;
std::unordered_map<std::shared_ptr<facebook::IAccessToken>, se::Object*>
    __jsb_s_accessTokens;
std::vector<std::shared_ptr<facebook::IAccessToken>> __jsb_s_accessTokenArchive;
} // namespace facebook

namespace core {
template <>
std::shared_ptr<facebook::IAccessToken>
get_value(const se::Value& value) {
    auto adPtr = static_cast<facebook::IAccessToken*>(
        value.toObject()->getPrivateData());
    auto iter = std::find_if(
        facebook::__jsb_s_accessTokenArchive.cbegin(),
        facebook::__jsb_s_accessTokenArchive.cend(),
        [=](const std::shared_ptr<facebook::IAccessToken>& ptr) -> bool {
            return adPtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_accessTokenArchive.cend()) {
        return *iter;
    } else {
        static std::shared_ptr<facebook::IAccessToken> defaultPtr;
        return defaultPtr;
    }
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::IAccessToken> input) {
    if (input != nullptr) {
        se::Object* obj = nullptr;
        if (facebook::__jsb_s_accessTokens.count(input) != 0) {
            obj = facebook::__jsb_s_accessTokens.at(input);
        } else {
            facebook::__jsb_s_accessTokenArchive.push_back(input);
            obj = se::Object::createObjectWithClass(
                facebook::__jsb_AccessToken_class);
            obj->setPrivateData(input.get());
        }
        value.setObject(obj);
    } else {
        value.setNull();
    }
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::IAccessToken>& input) {
    set_value<std::shared_ptr<facebook::IAccessToken>>(value, input);
}

template <>
bool jsb_finalize<facebook::IAccessToken>(se::State& s) {
    auto tokenPtr = static_cast<facebook::IAccessToken*>(s.nativeThisObject());
    auto iter = std::find_if(
        facebook::__jsb_s_accessTokenArchive.cbegin(),
        facebook::__jsb_s_accessTokenArchive.cend(),
        [=](const std::shared_ptr<facebook::IAccessToken>& ptr) -> bool {
            return tokenPtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_accessTokenArchive.cend()) {
        facebook::__jsb_s_accessTokenArchive.erase(iter);
    } else {
        delete tokenPtr;
    }
    return true;
}

template <>
se::Object*
create_JSON_object(const std::shared_ptr<facebook::IAccessToken>& value) {
    auto&& json = nlohmann::json();
    json.push_back(value->getToken());
    json.push_back(value->getApplicationId());
    json.push_back(value->getUserId());
    return se::Object::createJSONObject(json.dump());
}
} // namespace core

namespace facebook {
const auto jsb_AccessToken_finalize = &ee::core::jsb_finalize<IAccessToken>;
const auto jsb_AccessToken_getToken =
    &ee::core::jsb_method_get<IAccessToken, &IAccessToken::getToken,
                              const std::string&>;
const auto jsb_AccessToken_getApplicationId =
    &ee::core::jsb_method_get<IAccessToken, &IAccessToken::getApplicationId,
                              const std::string&>;
const auto jsb_AccessToken_getUserId =
    &ee::core::jsb_method_get<IAccessToken, &IAccessToken::getUserId,
                              const std::string&>;

SE_BIND_FINALIZE_FUNC(jsb_AccessToken_finalize);
SE_BIND_FUNC(jsb_AccessToken_getToken);
SE_BIND_FUNC(jsb_AccessToken_getApplicationId);
SE_BIND_FUNC(jsb_AccessToken_getUserId);

se::Class* getIAccessTokenClass() {
    CCASSERT(__jsb_AccessToken_class != nullptr,
             "__jsb_AccessToken_class is null");
    return __jsb_AccessToken_class;
}

bool register_ifacebook_access_token_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    ee::core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    ee::core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls = se::Class::create("IAccessToken", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_AccessToken_finalize));
    cls->defineFunction("getToken", _SE(jsb_AccessToken_getToken));
    cls->defineFunction("getApplicationId",
                        _SE(jsb_AccessToken_getApplicationId));
    cls->defineFunction("getUserId", _SE(jsb_AccessToken_getUserId));

    cls->install();

    JSBClassType::registerClass<IAccessToken>(cls);

    __jsb_AccessToken_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
} // namespace facebook
} // namespace ee
