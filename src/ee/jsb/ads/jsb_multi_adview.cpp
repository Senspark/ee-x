//
//  jsb_multi_adview.cpp
//  ee-x
//
//  Created by Kiet Le on 9/05/18.
//

#include "jsb_core_common.hpp"
#include "jsb_multi_adview.hpp"
#include <ee/Ads.hpp>

namespace ee {
namespace ads {

static se::Object* __jsb_Multi_AdView_proto = nullptr;
static se::Class* __jsb_Multi_AdView_class = nullptr;

constexpr auto jsb_Multi_AdView_finalize = &ee::core::jsb_finalize<MultiAdView>;
constexpr static auto jsb_Multi_AdView_contructor =
    &ee::core::jsb_constructor<ee::MultiAdView>;
constexpr auto jsb_Multi_AdView_isLoaded =
    &ee::core::jsb_accessor_get_on_ui_thread<MultiAdView,
                                             &MultiAdView::isLoaded, bool>;
constexpr auto jsb_Multi_AdView_load =
    &ee::core::jsb_method_call_on_ui_thread<MultiAdView, &MultiAdView::load>;
constexpr auto jsb_Multi_AdView_addItem =
    &ee::core::jsb_method_call_on_ui_thread<MultiAdView, &MultiAdView::addItem,
                                            std::shared_ptr<IAdView>>;
const static auto jsb_Multi_AdView_getAnchor =
    &ee::core::jsb_accessor_get_on_ui_thread<
        MultiAdView, &MultiAdView::getAnchor, std::pair<float, float>>;
const static auto jsb_Multi_AdView_setAnchor =
    &ee::core::jsb_method_call_on_ui_thread<
        MultiAdView, &MultiAdView::setAnchor, float, float>;
const static auto jsb_Multi_AdView_getPosition =
    &ee::core::jsb_accessor_get_on_ui_thread<
        MultiAdView, &MultiAdView::getPosition, std::pair<float, float>>;
const static auto jsb_Multi_AdView_setPosition =
    &ee::core::jsb_method_call_on_ui_thread<
        MultiAdView, &MultiAdView::setPosition, float, float>;
const static auto jsb_Multi_AdView_setPositionInPoints =
    &ee::core::jsb_method_call_on_ui_thread<
        MultiAdView, &MultiAdView::setPositionInPoints, float, float>;
const static auto jsb_Multi_AdView_getSize =
    &ee::core::jsb_accessor_get_on_ui_thread<
        MultiAdView, &MultiAdView::getSize,
        std::pair<std::int32_t, std::int32_t>>;
const static auto jsb_Multi_AdView_setSize =
    &ee::core::jsb_method_call_on_ui_thread<MultiAdView, &MultiAdView::setSize,
                                            std::int32_t, std::int32_t>;
const static auto jsb_Multi_AdView_setVisible =
    &ee::core::jsb_accessor_set_on_ui_thread<MultiAdView,
                                             &MultiAdView::setVisible, bool>;
const static auto jsb_Multi_AdView_setLoadCallback =
    &ee::core::jsb_set_callback<MultiAdView, &MultiAdView::setLoadCallback,
                                bool>;

SE_BIND_FINALIZE_FUNC(jsb_Multi_AdView_finalize)
SE_BIND_CTOR(jsb_Multi_AdView_contructor, __jsb_Multi_AdView_class,
             jsb_Multi_AdView_finalize)
SE_BIND_FUNC(jsb_Multi_AdView_isLoaded)
SE_BIND_FUNC(jsb_Multi_AdView_load)
SE_BIND_FUNC(jsb_Multi_AdView_addItem)
SE_BIND_FUNC(jsb_Multi_AdView_getAnchor)
SE_BIND_FUNC(jsb_Multi_AdView_setAnchor)
SE_BIND_FUNC(jsb_Multi_AdView_getPosition)
SE_BIND_FUNC(jsb_Multi_AdView_setPosition)
SE_BIND_FUNC(jsb_Multi_AdView_setPositionInPoints)
SE_BIND_FUNC(jsb_Multi_AdView_getSize)
SE_BIND_FUNC(jsb_Multi_AdView_setSize)
SE_BIND_FUNC(jsb_Multi_AdView_setVisible)
SE_BIND_FUNC(jsb_Multi_AdView_setLoadCallback)

bool register_multi_adview_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    core::getOrCreatePlainObject_r("ads", core::__eeObj, &adsObj);

    auto cls = se::Class::create("MultiAdView", adsObj, nullptr,
                                 _SE(jsb_Multi_AdView_contructor));
    cls->defineFinalizeFunction(_SE(jsb_Multi_AdView_finalize));

    cls->defineFunction("isLoaded", _SE(jsb_Multi_AdView_isLoaded));
    cls->defineFunction("load", _SE(jsb_Multi_AdView_load));
    cls->defineFunction("addItem", _SE(jsb_Multi_AdView_addItem));
    cls->defineFunction("getAnchor", _SE(jsb_Multi_AdView_getAnchor));
    cls->defineFunction("setAnchor", _SE(jsb_Multi_AdView_setAnchor));
    cls->defineFunction("getPosition", _SE(jsb_Multi_AdView_getPosition));
    cls->defineFunction("setPosition", _SE(jsb_Multi_AdView_setPosition));
    cls->defineFunction("setPositionInPoints",
                        _SE(jsb_Multi_AdView_setPositionInPoints));
    cls->defineFunction("getSize", _SE(jsb_Multi_AdView_getSize));
    cls->defineFunction("setSize", _SE(jsb_Multi_AdView_setSize));
    cls->defineFunction("setVisible", _SE(jsb_Multi_AdView_setVisible));
    cls->defineFunction("setLoadCallback",
                        _SE(jsb_Multi_AdView_setLoadCallback));

    cls->install();

    JSBClassType::registerClass<ee::MultiAdView>(cls);

    __jsb_Multi_AdView_proto = cls->getProto();
    __jsb_Multi_AdView_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}

} // namespace ads
} // namespace ee
