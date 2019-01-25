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
namespace core {
namespace {
std::unique_ptr<SharedPtrHandler<facebook::IAccessToken>> handler;
} // namespace

template <>
std::shared_ptr<facebook::IAccessToken> get_value(const se::Value& value) {
    return handler->getValue(value);
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::IAccessToken> input) {
    handler->setValue(value, input);
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::IAccessToken>& input) {
    handler->setValue(value, input);
}

template <>
bool makeFinalize<facebook::IAccessToken>(se::State& state) {
    return handler->finalize(state);
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
namespace {
using Self = IAccessToken;

// clang-format off
constexpr auto finalize         = &core::makeFinalize<Self>;
constexpr auto getToken         = &core::makeInstanceMethod<&Self::getToken>;
constexpr auto getApplicationId = &core::makeInstanceMethod<&Self::getApplicationId>;
constexpr auto getUserId        = &core::makeInstanceMethod<&Self::getUserId>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize);
SE_BIND_FUNC(getToken);
SE_BIND_FUNC(getApplicationId);
SE_BIND_FUNC(getUserId);
} // namespace

se::Class* getIAccessTokenClass() {
    CCASSERT(core::handler != nullptr, "__jsb_AccessToken_class is null");
    return core::handler->getClass();
}

bool register_ifacebook_access_token_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    ee::core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    ee::core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls = se::Class::create("IAccessToken", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, getToken);
    EE_JSB_DEFINE_FUNCTION(cls, getApplicationId);
    EE_JSB_DEFINE_FUNCTION(cls, getUserId);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    core::handler = core::SharedPtrHandler<Self>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
