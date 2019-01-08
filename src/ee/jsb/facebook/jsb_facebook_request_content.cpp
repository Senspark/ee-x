//
//  jsb_facebook_request_content.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "jsb_facebook_request_content.hpp"

#include "ee/facebook/FacebookRequestContent.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace facebook {
se::Class* __jsb_RequestContent_class = nullptr;
std::unordered_map<std::shared_ptr<RequestContent>, se::Object*>
    __jsb_s_requestContents;
std::vector<std::shared_ptr<RequestContent>> __jsb_s_requestContentsArchive;
} // namespace facebook
namespace core {
template <>
facebook::RequestContent::ActionType get_value(const se::Value& value) {
    return static_cast<facebook::RequestContent::ActionType>(value.toInt32());
}

template <>
facebook::RequestContent::Filter get_value(const se::Value& value) {
    return static_cast<facebook::RequestContent::Filter>(value.toInt32());
}

template <>
std::shared_ptr<facebook::RequestContent> get_value(const se::Value& value) {
    auto itemPtr = static_cast<facebook::RequestContent*>(
        value.toObject()->getPrivateData());
    auto iter = std::find_if(
        facebook::__jsb_s_requestContentsArchive.cbegin(),
        facebook::__jsb_s_requestContentsArchive.cend(),
        [=](const std::shared_ptr<facebook::RequestContent>& ptr) -> bool {
            return itemPtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_requestContentsArchive.cend()) {
        return *iter;
    } else {
        return std::shared_ptr<facebook::RequestContent>(itemPtr);
    }
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::RequestContent> input) {
    if (input != nullptr) {
        se::Object* obj = nullptr;
        if (facebook::__jsb_s_requestContents.count(input) != 0) {
            obj = facebook::__jsb_s_requestContents.at(input);
        } else {
            facebook::__jsb_s_requestContentsArchive.push_back(input);
            obj = se::Object::createObjectWithClass(
                facebook::__jsb_RequestContent_class);
            obj->setPrivateData(input.get());
        }
        value.setObject(obj);
    } else {
        value.setNull();
    }
}

// FIXME pls check this function
template <>
facebook::RequestContent get_value(const se::Value& value) {
    auto ptr = get_value<std::shared_ptr<facebook::RequestContent>>(value);
    return *ptr.get();
}

template <>
void set_value(se::Value& value, const facebook::RequestContent& input) {
    auto ptr = std::make_shared<facebook::RequestContent>(input);
    set_value<std::shared_ptr<facebook::RequestContent>>(value, ptr);
}

template <>
bool jsb_finalize<facebook::RequestContent>(se::State& s) {
    auto itemPtr = static_cast<facebook::RequestContent*>(s.nativeThisObject());
    auto iter = std::find_if(
        facebook::__jsb_s_requestContentsArchive.cbegin(),
        facebook::__jsb_s_requestContentsArchive.cend(),
        [=](const std::shared_ptr<facebook::RequestContent>& ptr) -> bool {
            return itemPtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_requestContentsArchive.cend()) {
        facebook::__jsb_s_requestContentsArchive.erase(iter);
    } else {
        delete itemPtr;
    }
    return true;
}
} // namespace core

namespace facebook {
const auto jsb_RequestContent_finalize =
    &ee::core::jsb_finalize<RequestContent>;
const auto jsb_RequestContent_constructor =
    &ee::core::jsb_constructor<RequestContent>;
const auto jsb_RequestContent_setActionType =
    &ee::core::jsb_accessor_set<RequestContent, &RequestContent::setActionType,
                                RequestContent::ActionType>;
const auto jsb_RequestContent_setFilter =
    &ee::core::jsb_accessor_set<RequestContent, &RequestContent::setFilter,
                                RequestContent::Filter>;
const auto jsb_RequestContent_setRecipients =
    &ee::core::jsb_accessor_set<RequestContent, &RequestContent::setRecipients,
                                const std::vector<std::string>&>;
const auto jsb_RequestContent_setObjectId =
    &ee::core::jsb_accessor_set<RequestContent, &RequestContent::setObjectId,
                                const std::string&>;
const auto jsb_RequestContent_setTitle =
    &ee::core::jsb_accessor_set<RequestContent, &RequestContent::setTitle,
                                const std::string&>;
const auto jsb_RequestContent_setMessage =
    &ee::core::jsb_accessor_set<RequestContent, &RequestContent::setMessage,
                                const std::string&>;
const auto jsb_RequestContent_setData =
    &ee::core::jsb_accessor_set<RequestContent, &RequestContent::setData,
                                const std::string&>;
const auto jsb_RequestContent_toString =
    &ee::core::jsb_method_get<RequestContent, &RequestContent::toString,
                              std::string>;

SE_BIND_FINALIZE_FUNC(jsb_RequestContent_finalize);
SE_BIND_CTOR(jsb_RequestContent_constructor, __jsb_RequestContent_class,
             jsb_RequestContent_finalize);
SE_BIND_FUNC(jsb_RequestContent_setActionType);
SE_BIND_FUNC(jsb_RequestContent_setFilter);
SE_BIND_FUNC(jsb_RequestContent_setRecipients);
SE_BIND_FUNC(jsb_RequestContent_setObjectId);
SE_BIND_FUNC(jsb_RequestContent_setTitle);
SE_BIND_FUNC(jsb_RequestContent_setMessage);
SE_BIND_FUNC(jsb_RequestContent_setData);
SE_BIND_FUNC(jsb_RequestContent_toString);

bool register_facebook_request_content_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    se::Object* reqContentObj = nullptr;
    se::Object* actionTypeObj = nullptr;
    se::Object* filterObj = nullptr;

    ee::core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    ee::core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);
    ee::core::getOrCreatePlainObject_r("RequestContent", facebookObj,
                                       &reqContentObj);
    ee::core::getOrCreatePlainObject_r("ActionType", reqContentObj,
                                       &actionTypeObj);
    ee::core::getOrCreatePlainObject_r("Filter", reqContentObj, &filterObj);

    actionTypeObj->setProperty("None", se::Value(static_cast<std::int32_t>(
                                           RequestContent::ActionType::None)));
    actionTypeObj->setProperty("Send", se::Value(static_cast<std::int32_t>(
                                           RequestContent::ActionType::Send)));
    actionTypeObj->setProperty("AskFor",
                               se::Value(static_cast<std::int32_t>(
                                   RequestContent::ActionType::AskFor)));
    actionTypeObj->setProperty("Turn", se::Value(static_cast<std::int32_t>(
                                           RequestContent::ActionType::Turn)));

    filterObj->setProperty("None", se::Value(static_cast<std::int32_t>(
                                       RequestContent::Filter::None)));
    filterObj->setProperty("AppUsers", se::Value(static_cast<std::int32_t>(
                                           RequestContent::Filter::AppUsers)));
    filterObj->setProperty("AppNonUsers",
                           se::Value(static_cast<std::int32_t>(
                               RequestContent::Filter::AppNonUsers)));

    auto cls = se::Class::create("RequestContent", facebookObj, nullptr,
                                 _SE(jsb_RequestContent_constructor));
    cls->defineFinalizeFunction(_SE(jsb_RequestContent_finalize));
    cls->defineFunction("setActionType", _SE(jsb_RequestContent_setActionType));
    cls->defineFunction("setFilter", _SE(jsb_RequestContent_setFilter));
    cls->defineFunction("setRecipients", _SE(jsb_RequestContent_setRecipients));
    cls->defineFunction("setObjectId", _SE(jsb_RequestContent_setObjectId));
    cls->defineFunction("setTitle", _SE(jsb_RequestContent_setTitle));
    cls->defineFunction("setMessage", _SE(jsb_RequestContent_setMessage));
    cls->defineFunction("setData", _SE(jsb_RequestContent_setData));
    cls->defineFunction("toString", _SE(jsb_RequestContent_toString));

    cls->install();

    JSBClassType::registerClass<RequestContent>(cls);

    __jsb_RequestContent_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
} // namespace facebook
} // namespace ee
