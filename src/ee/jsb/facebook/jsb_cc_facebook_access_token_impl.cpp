//
//  jsb_cc_facebook_access_token_impl.cpp
//  ee-x
//
//  Created by senspark-dev on 12/13/18.
//

#include "jsb_cc_facebook_access_token_impl.hpp"

#include "ee/facebook/internal/FacebookAccessToken.hpp"

#include "jsb_cc_facebook_iaccess_token.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace facebook {
se::Class* __jsb_AccessTokenImpl_class = nullptr;

constexpr auto jsb_AccessTokenImpl_finalize = &core::jsb_finalize<AccessToken>;
constexpr auto jsb_AccessTokenImpl_constructor =
    &core::jsb_constructor<AccessToken, const std::string&>;

SE_BIND_FINALIZE_FUNC(jsb_AccessTokenImpl_finalize);
SE_BIND_CTOR(jsb_AccessTokenImpl_constructor, __jsb_AccessTokenImpl_class,
             jsb_AccessTokenImpl_finalize);

bool register_facebook_access_token_impl(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;

    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls = se::Class::create("AccessToken", facebookObj,
                                 getIAccessTokenClass()->getProto(),
                                 _SE(jsb_AccessTokenImpl_constructor));
    cls->defineFinalizeFunction(_SE(jsb_AccessTokenImpl_finalize));

    cls->install();

    JSBClassType::registerClass<AccessToken>(cls);

    __jsb_AccessTokenImpl_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
} // namespace facebook
} // namespace ee
