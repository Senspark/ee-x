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

namespace ee {

namespace ads {
static se::Object* __jsb_AdView_proto = nullptr;
static se::Class* __jsb_AdView_class = nullptr;

static std::unordered_map<std::shared_ptr<ee::IAdView>, se::Object*>
    __jsb_s_adviews;
static std::vector<std::shared_ptr<ee::IAdView>> __jsb_s_adviewArchive;
} // namespace ads

namespace core {
template <>
std::shared_ptr<ee::IAdView> get_value(const se::Value& value) {
    auto data = static_cast<ee::IAdView*>(value.toObject()->getPrivateData());
    return std::shared_ptr<ee::IAdView>(data);
}

template <>
void set_value(se::Value& value, std::shared_ptr<ee::IAdView> input) {
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

se::Object* __adsObj = nullptr;

const static auto jsb_AdView_finalize = &ee::core::jsb_finalize<IAdView>;
const static auto jsb_AdView_isLoaded =
    &ee::core::jsb_accessor_get_on_ui_thread<IAdView, &IAdView::isLoaded, bool>;
const static auto jsb_AdView_load =
    &ee::core::jsb_method_call_on_ui_thread<IAdView, &IAdView::load>;
const static auto jsb_AdView_getAnchor =
    &ee::core::jsb_accessor_get_on_ui_thread<IAdView, &IAdView::getAnchor,
                                             const std::pair<float, float>&>;
const static auto jsb_AdView_setAnchor =
    &ee::core::jsb_method_call_on_ui_thread<IAdView, &IAdView::setAnchor, float,
                                            float>;
const static auto jsb_AdView_getPosition =
    &ee::core::jsb_accessor_get_on_ui_thread<IAdView, &IAdView::getPosition,
                                             const std::pair<float, float>&>;
const static auto jsb_AdView_setPosition =
    &ee::core::jsb_method_call_on_ui_thread<IAdView, &IAdView::setPosition,
                                            float, float>;
const static auto jsb_AdView_getSize = &ee::core::jsb_accessor_get_on_ui_thread<
    IAdView, &IAdView::getSize, const std::pair<std::int32_t, std::int32_t>&>;
const static auto jsb_AdView_setSize =
    &ee::core::jsb_method_call_on_ui_thread<IAdView, &IAdView::setSize,
                                            std::int32_t, std::int32_t>;
const static auto jsb_AdView_setVisible =
    &ee::core::jsb_accessor_set_on_ui_thread<IAdView, &IAdView::setVisible,
                                             bool>;
const static auto jsb_AdView_setLoadCallback =
    &ee::core::jsb_set_callback<IAdView, &IAdView::setLoadCallback, bool>;

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

bool register_adview_manual(se::Object* globalObj) {
    core::getOrCreatePlainObject_r("ads", globalObj, &__adsObj);

    auto cls = se::Class::create("AdView", __adsObj, nullptr, nullptr);
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

    JSBClassType::registerClass<ee::IAdView>(cls);

    __jsb_AdView_proto = cls->getProto();
    __jsb_AdView_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace ads
} // namespace ee
