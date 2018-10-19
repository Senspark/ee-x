//
// jsb_iron_source_bridge.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 8/24/18
//
//

#include "ee/jsb/ironsource/jsb_iron_source_bridge.hpp"

#include "ee/IronSource.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {

namespace ironsource {

static se::Object* __jsb_IronSource_proto = nullptr;
static se::Class* __jsb_IronSource_class = nullptr;

static se::Object* __ironsourceObj = nullptr;

const auto jsb_IronSource_finalize = &core::jsb_finalize<IronSource>;
const auto jsb_IronSource_constructor = &core::jsb_constructor<IronSource>;
const auto jsb_IronSource_initialize =
    &core::jsb_method_call_on_ui_thread_and_wait<
        IronSource, &IronSource::initialize, const std::string&>;
const auto jsb_IronSource_createRewardedVideo =
    &core::jsb_method_get_on_ui_thread<
        IronSource, &IronSource::createRewardedVideo,
        std::shared_ptr<IRewardedVideo>, const std::string&>;
const auto jsb_IronSource_createInterstitialAd =
    &core::jsb_method_get_on_ui_thread<
        IronSource, &IronSource::createInterstitialAd,
        std::shared_ptr<IInterstitialAd>, const std::string&>;

SE_BIND_FINALIZE_FUNC(jsb_IronSource_finalize)
SE_BIND_CTOR(jsb_IronSource_constructor, __jsb_IronSource_class,
             jsb_IronSource_finalize)
SE_BIND_FUNC(jsb_IronSource_initialize)
SE_BIND_FUNC(jsb_IronSource_createRewardedVideo)
SE_BIND_FUNC(jsb_IronSource_createInterstitialAd)

bool register_iron_source_bridge_manual(se::Object* globalObj) {
    core::getOrCreatePlainObject_r("ironsource", core::__eeObj,
                                   &__ironsourceObj);

    auto cls = se::Class::create("IronSource", __ironsourceObj, nullptr,
                                 _SE(jsb_IronSource_constructor));
    cls->defineFinalizeFunction(_SE(jsb_IronSource_finalize));

    cls->defineFunction("initialize", _SE(jsb_IronSource_initialize));
    cls->defineFunction("createRewardedVideo",
                        _SE(jsb_IronSource_createRewardedVideo));
    cls->defineFunction("createInterstitialAd",
                        _SE(jsb_IronSource_createInterstitialAd));

    cls->install();

    JSBClassType::registerClass<ee::IronSource>(cls);

    __jsb_IronSource_proto = cls->getProto();
    __jsb_IronSource_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace ironsource
} // namespace ee
