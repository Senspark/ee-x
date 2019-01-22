//
//  jsb_adview.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#include "ee/jsb/ads/jsb_adview.hpp"

#include <unordered_map>

#include <ee/Ads.hpp>

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace ads {
se::Class* __jsb_AdView_class = nullptr;
std::unordered_map<std::shared_ptr<IAdView>, se::Object*> __jsb_s_adviews;
std::vector<std::shared_ptr<IAdView>> __jsb_s_adviewArchive;
} // namespace ads

namespace core {
template <>
std::shared_ptr<IAdView> get_value(const se::Value& value) {
    auto adPtr = static_cast<IAdView*>(value.toObject()->getPrivateData());
    auto iter = std::find_if(ads::__jsb_s_adviewArchive.cbegin(),
                             ads::__jsb_s_adviewArchive.cend(),
                             [=](const std::shared_ptr<IAdView>& ptr) -> bool {
                                 return adPtr == ptr.get();
                             });
    if (iter != ads::__jsb_s_adviewArchive.cend()) {
        return *iter;
    } else {
        return std::shared_ptr<IAdView>(adPtr);
    }
}

template <>
void set_value(se::Value& value, std::shared_ptr<IAdView> input) {
    if (input != nullptr) {
        se::Object* obj = nullptr;
        if (ads::__jsb_s_adviews.count(input) != 0) {
            obj = ads::__jsb_s_adviews.at(input);
        } else {
            ads::__jsb_s_adviewArchive.push_back(input);
            obj = se::Object::createObjectWithClass(ads::__jsb_AdView_class);
            obj->setPrivateData(input.get());
        }
        value.setObject(obj);
    } else {
        value.setNull();
    }
}

template <>
void set_value(se::Value& value, std::shared_ptr<IAdView>& input) {
    set_value<std::shared_ptr<IAdView>>(value, input);
}

template <>
bool jsb_finalize<IAdView>(se::State& s) {
    auto adviewPtr = static_cast<IAdView*>(s.nativeThisObject());
    auto iter = std::find_if(ads::__jsb_s_adviewArchive.cbegin(),
                             ads::__jsb_s_adviewArchive.cend(),
                             [=](const std::shared_ptr<IAdView>& ptr) -> bool {
                                 return adviewPtr == ptr.get();
                             });
    if (iter != ads::__jsb_s_adviewArchive.cend()) {
        ads::__jsb_s_adviewArchive.erase(iter);
    } else {
        delete adviewPtr;
    }
    return true;
}
} // namespace core

namespace ads {

const auto jsb_AdView_finalize = &core::jsb_finalize<IAdView>;
const auto jsb_AdView_isLoaded =
    &core::jsb_accessor_get_on_ui_thread<IAdView, &IAdView::isLoaded, bool>;
const auto jsb_AdView_load =
    &core::jsb_method_call_on_ui_thread<IAdView, &IAdView::load>;
const auto jsb_AdView_getAnchor =
    &core::jsb_accessor_get_on_ui_thread<IAdView, &IAdView::getAnchor,
                                         const std::pair<float, float>&>;
const auto jsb_AdView_setAnchor =
    &core::jsb_method_call_on_ui_thread<IAdView, &IAdView::setAnchor, float,
                                        float>;
const auto jsb_AdView_getPosition =
    &core::jsb_accessor_get_on_ui_thread<IAdView, &IAdView::getPosition,
                                         const std::pair<float, float>&>;
const auto jsb_AdView_setPosition =
    &core::jsb_method_call_on_ui_thread<IAdView, &IAdView::setPosition, float,
                                        float>;
const auto jsb_AdView_getSize = &core::jsb_accessor_get_on_ui_thread<
    IAdView, &IAdView::getSize, const std::pair<std::int32_t, std::int32_t>&>;
const auto jsb_AdView_setSize =
    &core::jsb_method_call_on_ui_thread<IAdView, &IAdView::setSize,
                                        std::int32_t, std::int32_t>;
const auto jsb_AdView_setVisible =
    &core::jsb_accessor_set_on_ui_thread<IAdView, &IAdView::setVisible, bool>;
const auto jsb_AdView_setLoadCallback =
    &core::jsb_set_callback<IAdView, &IAdView::setLoadCallback, bool>;

SE_BIND_FINALIZE_FUNC(jsb_AdView_finalize)
SE_BIND_FUNC(jsb_AdView_isLoaded)
SE_BIND_FUNC(jsb_AdView_load)
SE_BIND_FUNC(jsb_AdView_getAnchor)
SE_BIND_FUNC(jsb_AdView_setAnchor)
SE_BIND_FUNC(jsb_AdView_getPosition)
SE_BIND_FUNC(jsb_AdView_setPosition)
SE_BIND_FUNC(jsb_AdView_getSize)
SE_BIND_FUNC(jsb_AdView_setSize)
SE_BIND_FUNC(jsb_AdView_setVisible)
SE_BIND_FUNC(jsb_AdView_setLoadCallback)

se::Class* getIAdViewClass() {
    CCASSERT(__jsb_AdView_class != nullptr, "__jsb_AdView_class is null");
    return __jsb_AdView_class;
}

bool register_adview_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* adsObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("ads", eeObj, &adsObj);

    auto cls = se::Class::create("IAdView", adsObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_AdView_finalize));

    cls->defineFunction("isLoaded", _SE(jsb_AdView_isLoaded));
    cls->defineFunction("load", _SE(jsb_AdView_load));
    cls->defineFunction("getAnchor", _SE(jsb_AdView_getAnchor));
    cls->defineFunction("setAnchor", _SE(jsb_AdView_setAnchor));
    cls->defineFunction("getPosition", _SE(jsb_AdView_getPosition));
    cls->defineFunction("setPosition", _SE(jsb_AdView_setPosition));
    cls->defineFunction("getSize", _SE(jsb_AdView_getSize));
    cls->defineFunction("setSize", _SE(jsb_AdView_setSize));
    cls->defineFunction("setVisible", _SE(jsb_AdView_setVisible));
    cls->defineFunction("setLoadCallback", _SE(jsb_AdView_setLoadCallback));

    cls->install();

    JSBClassType::registerClass<IAdView>(cls);

    __jsb_AdView_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace ads
} // namespace ee
