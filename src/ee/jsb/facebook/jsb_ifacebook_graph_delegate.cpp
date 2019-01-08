//
//  jsb_ifacebook_graph_delegate.cpp
//  Pods
//
//  Created by senspark-dev on 1/8/19.
//

#include "jsb_ifacebook_graph_delegate.hpp"

#include "ee/facebook/IFacebookDelegate.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace facebook {
se::Class* __jsb_GraphDelegate_class = nullptr;
std::unordered_map<std::shared_ptr<IGraphDelegate>, se::Object*>
    __jsb_s_fbGraphDelegates;
std::vector<std::shared_ptr<IGraphDelegate>> __jsb_s_fbGraphDelegateArchive;
} // namespace facebook

namespace core {
template <>
std::shared_ptr<facebook::IGraphDelegate> get_value(const se::Value& value) {
    auto delegatePtr = static_cast<facebook::IGraphDelegate*>(
        value.toObject()->getPrivateData());
    auto iter = std::find_if(
        facebook::__jsb_s_fbGraphDelegateArchive.cbegin(),
        facebook::__jsb_s_fbGraphDelegateArchive.cend(),
        [=](const std::shared_ptr<facebook::IGraphDelegate>& ptr) -> bool {
            return delegatePtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_fbGraphDelegateArchive.cend()) {
        return *iter;
    } else {
        return std::shared_ptr<facebook::IGraphDelegate>(delegatePtr);
    }
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::IGraphDelegate> input) {
    if (input != nullptr) {
        se::Object* obj = nullptr;
        if (facebook::__jsb_s_fbGraphDelegates.count(input) != 0) {
            obj = facebook::__jsb_s_fbGraphDelegates.at(input);
        } else {
            facebook::__jsb_s_fbGraphDelegateArchive.push_back(input);
            obj = se::Object::createObjectWithClass(
                facebook::__jsb_GraphDelegate_class);
            obj->setPrivateData(input.get());
        }
        value.setObject(obj);
    } else {
        value.setNull();
    }
}

template <>
bool jsb_finalize<facebook::IGraphDelegate>(se::State& s) {
    auto delegatePtr =
        static_cast<facebook::IGraphDelegate*>(s.nativeThisObject());
    auto iter = std::find_if(
        facebook::__jsb_s_fbGraphDelegateArchive.cbegin(),
        facebook::__jsb_s_fbGraphDelegateArchive.cend(),
        [=](const std::shared_ptr<facebook::IGraphDelegate>& ptr) -> bool {
            return delegatePtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_fbGraphDelegateArchive.cend()) {
        facebook::__jsb_s_fbGraphDelegateArchive.erase(iter);
    } else {
        delete delegatePtr;
    }
    return true;
}
} // namespace core

namespace facebook {
const auto jsb_GraphDelegate_finalize =
    &ee::core::jsb_finalize<IGraphDelegate>;
const auto jsb_GraphDelegate_onSuccess =
    &ee::core::jsb_set_callback<IGraphDelegate,
                                &IGraphDelegate::onSuccess,
                                const std::string&>;
const auto jsb_GraphDelegate_onFailure =
    &ee::core::jsb_set_callback<IGraphDelegate,
                                &IGraphDelegate::onFailure,
                                const std::string&>;
const auto jsb_GraphDelegate_onCancel =
    &ee::core::jsb_set_callback<IGraphDelegate,
                                &IGraphDelegate::onCancel>;

SE_BIND_FINALIZE_FUNC(jsb_GraphDelegate_finalize);
SE_BIND_FUNC(jsb_GraphDelegate_onSuccess);
SE_BIND_FUNC(jsb_GraphDelegate_onFailure);
SE_BIND_FUNC(jsb_GraphDelegate_onCancel);

bool register_ifacebook_graph_delegate_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    ee::core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    ee::core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls = se::Class::create("IGraphDelegate", facebookObj, nullptr,
                                 nullptr);
    cls->defineFinalizeFunction(_SE(jsb_GraphDelegate_finalize));
    cls->defineFunction("onSuccess", _SE(jsb_GraphDelegate_onSuccess));
    cls->defineFunction("onFailure", _SE(jsb_GraphDelegate_onFailure));
    cls->defineFunction("onCancel", _SE(jsb_GraphDelegate_onCancel));

    cls->install();

    JSBClassType::registerClass<IGraphDelegate>(cls);

    __jsb_GraphDelegate_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
} // namespace facebook
} // namespace ee
