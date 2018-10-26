//
//  jsb_multi_adview.cpp
//  ee-x
//
//  Created by Kiet Le on 9/05/18.
//

#include "ee/jsb/ads/jsb_multi_adview.hpp"

#include <ee/Ads.hpp>

#include "ee/jsb/ads/jsb_adview.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace ads {

se::Class* __jsb_Multi_AdView_class = nullptr;

constexpr auto jsb_Multi_AdView_finalize = &core::jsb_finalize<MultiAdView>;
constexpr auto jsb_Multi_AdView_contructor =
    &core::jsb_constructor<MultiAdView>;
constexpr auto jsb_Multi_AdView_addItem =
    &core::jsb_method_call_on_ui_thread<MultiAdView, &MultiAdView::addItem,
                                        std::shared_ptr<IAdView>>;

SE_BIND_FINALIZE_FUNC(jsb_Multi_AdView_finalize)
SE_BIND_CTOR(jsb_Multi_AdView_contructor, __jsb_Multi_AdView_class,
             jsb_Multi_AdView_finalize)
SE_BIND_FUNC(jsb_Multi_AdView_addItem)

bool register_multi_adview_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    se::Object* eeObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("ads", eeObj, &adsObj);

    auto cls =
        se::Class::create("MultiAdView", adsObj, getIAdViewClass()->getProto(),
                          _SE(jsb_Multi_AdView_contructor));
    cls->defineFinalizeFunction(_SE(jsb_Multi_AdView_finalize));

    cls->defineFunction("addItem", _SE(jsb_Multi_AdView_addItem));

    cls->install();

    JSBClassType::registerClass<MultiAdView>(cls);

    __jsb_Multi_AdView_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}

} // namespace ads
} // namespace ee
