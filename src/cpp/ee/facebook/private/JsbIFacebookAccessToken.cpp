//
//  jsb_ifacebook_access_token.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/facebook/private/JsbIFacebookAccessToken.hpp"

#ifdef EE_X_COCOS_JS
#include <ee/core/JsbUtils.hpp>
#include <ee/core/internal/SharedPtrHandler.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/facebook/IFacebookAccessToken.hpp"

using Self = ee::facebook::IAccessToken;

namespace ee {
namespace core {
namespace {
std::unique_ptr<SharedPtrHandler<Self>> handler;
} // namespace

template <>
std::shared_ptr<Self> get_value(const se::Value& value) {
    return handler->getValue(value);
}

template <>
void set_value(se::Value& value, std::shared_ptr<Self> input) {
    handler->setValue(value, input);
}

template <>
void set_value(se::Value& value, std::shared_ptr<Self>& input) {
    handler->setValue(value, input);
}

template <>
bool makeFinalize<Self>(se::State& state) {
    return handler->finalize(state);
}

template <>
se::HandleObject create_JSON_object(const std::shared_ptr<Self>& value) {
    auto&& json = nlohmann::json();
    json.push_back(value->getToken());
    json.push_back(value->getApplicationId());
    json.push_back(value->getUserId());
    return se::HandleObject(se::Object::createJSONObject(json.dump()));
}
} // namespace core

namespace facebook {
namespace {
// clang-format off
constexpr auto finalize         = &core::makeFinalize<Self>;
constexpr auto getToken         = &core::makeMethod<&Self::getToken>;
constexpr auto getApplicationId = &core::makeMethod<&Self::getApplicationId>;
constexpr auto getUserId        = &core::makeMethod<&Self::getUserId>;
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

bool registerJsbIAccessToken(se::Object* global) {
    auto scope = JsbUtils::getPath(global, "ee");
    auto cls =
        se::Class::create("IFacebookAccessToken", scope, nullptr, nullptr);
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

#endif // EE_X_COCOS_JS