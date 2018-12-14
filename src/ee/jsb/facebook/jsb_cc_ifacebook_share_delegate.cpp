//
//  jsb_cc_ifacebook_share_delegate.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "jsb_cc_ifacebook_share_delegate.hpp"

#include "ee/facebook/IFacebookDelegate.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace facebook {
se::Class* __jsb_ShareDelegate_class = nullptr;
std::unordered_map<std::shared_ptr<IShareDelegate>, se::Object*>
    __jsb_s_fbShareDelegates;
std::vector<std::shared_ptr<IShareDelegate>> __jsb_s_fbShareDelegateArchive;
} // namespace facebook

namespace core {
template <>
std::shared_ptr<facebook::IShareDelegate> get_value(const se::Value& value) {
    auto delegatePtr = static_cast<facebook::IGraphDelegate*>(
        value.toObject()->getPrivateData());
    auto iter = std::find_if(
        facebook::__jsb_s_fbShareDelegateArchive.cbegin(),
        facebook::__jsb_s_fbShareDelegateArchive.cend(),
        [=](const std::shared_ptr<facebook::IShareDelegate>& ptr) -> bool {
            return delegatePtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_fbShareDelegateArchive.cend()) {
        return *iter;
    } else {
        return std::shared_ptr<facebook::IShareDelegate>(delegatePtr);
    }
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::IShareDelegate> input) {
    if (input != nullptr) {
        se::Object* obj = nullptr;
        if (facebook::__jsb_s_fbShareDelegates.count(input) != 0) {
            obj = facebook::__jsb_s_fbShareDelegates.at(input);
        } else {
            facebook::__jsb_s_fbShareDelegateArchive.push_back(input);
            obj = se::Object::createObjectWithClass(
                facebook::__jsb_ShareDelegate_class);
            obj->setPrivateData(input.get());
        }
        value.setObject(obj);
    } else {
        value.setNull();
    }
}

template <>
bool jsb_finalize<facebook::IShareDelegate>(se::State& s) {
    auto delegatePtr =
        static_cast<facebook::IShareDelegate*>(s.nativeThisObject());
    auto iter = std::find_if(
        facebook::__jsb_s_fbShareDelegateArchive.cbegin(),
        facebook::__jsb_s_fbShareDelegateArchive.cend(),
        [=](const std::shared_ptr<facebook::IShareDelegate>& ptr) -> bool {
            return delegatePtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_fbShareDelegateArchive.cend()) {
        facebook::__jsb_s_fbShareDelegateArchive.erase(iter);
    } else {
        delete delegatePtr;
    }
    return true;
}
} // namespace core

namespace facebook {
const auto jsb_ShareDelegate_finalize =
    &ee::core::jsb_finalize<IFacebookShareDelegate>;
const auto jsb_ShareDelegate_onSuccess =
    &ee::core::jsb_set_callback<IFacebookShareDelegate,
                                &IFacebookShareDelegate::onSuccess,
                                const std::string&>;
const auto jsb_ShareDelegate_onFailure =
    &ee::core::jsb_set_callback<IFacebookShareDelegate,
                                &IFacebookShareDelegate::onFailure,
                                const std::string&>;
const auto jsb_ShareDelegate_onCancel =
    &ee::core::jsb_set_callback<IFacebookShareDelegate,
                                &IFacebookShareDelegate::onCancel>;

SE_BIND_FINALIZE_FUNC(jsb_ShareDelegate_finalize);
SE_BIND_FUNC(jsb_ShareDelegate_onSuccess);
SE_BIND_FUNC(jsb_ShareDelegate_onFailure);
SE_BIND_FUNC(jsb_ShareDelegate_onCancel);

bool register_ifacebook_share_delegate_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    ee::core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    ee::core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls = se::Class::create("IFacebookShareDelegate", facebookObj, nullptr,
                                 nullptr);
    cls->defineFinalizeFunction(_SE(jsb_ShareDelegate_finalize));
    cls->defineFunction("onSuccess", _SE(jsb_ShareDelegate_onSuccess));
    cls->defineFunction("onFailure", _SE(jsb_ShareDelegate_onFailure));
    cls->defineFunction("onCancel", _SE(jsb_ShareDelegate_onCancel));

    cls->install();

    JSBClassType::registerClass<IFacebookShareDelegate>(cls);

    __jsb_ShareDelegate_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
} // namespace facebook
} // namespace ee
