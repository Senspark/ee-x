//
//  jsb_multi_rewarded_video.cpp
//  ee-x
//
//  Created by Kiet Le on 9/04/18.
//

#include "ee/jsb/ads/jsb_multi_rewarded_video.hpp"

#include <unordered_map>

#include <ee/Ads.hpp>

#include "ee/jsb/core/jsb_core_common.hpp"

namespace ee {
namespace ads {

static se::Object* __jsb_Multi_Rewarded_Video_proto = nullptr;
static se::Class* __jsb_Multi_Rewarded_Video_class = nullptr;

constexpr auto jsb_Multi_Rewarded_Video_finalize =
    &ee::core::jsb_finalize<MultiRewardedVideo>;
constexpr static auto jsb_Multi_Rewarded_Video_contructor =
    &ee::core::jsb_constructor<ee::MultiRewardedVideo>;
constexpr auto jsb_Multi_Rewarded_Video_isLoaded =
    &ee::core::jsb_accessor_get_on_ui_thread<
        MultiRewardedVideo, &MultiRewardedVideo::isLoaded, bool>;
constexpr auto jsb_Multi_Rewarded_Video_load =
    &ee::core::jsb_method_call_on_ui_thread<MultiRewardedVideo,
                                            &MultiRewardedVideo::load>;
constexpr auto jsb_Multi_Rewarded_Video_show =
    &ee::core::jsb_method_call_on_ui_thread<MultiRewardedVideo,
                                            &MultiRewardedVideo::show>;
constexpr auto jsb_Multi_Rewarded_Video_addItem =
    &ee::core::jsb_method_call_on_ui_thread<MultiRewardedVideo,
                                            &MultiRewardedVideo::addItem,
                                            std::shared_ptr<IRewardedVideo>>;
constexpr auto jsb_Multi_Rewarded_Video_setResultCallback =
    &ee::core::jsb_set_callback<MultiRewardedVideo,
                                &MultiRewardedVideo::setResultCallback, bool>;

SE_BIND_FINALIZE_FUNC(jsb_Multi_Rewarded_Video_finalize)
SE_BIND_CTOR(jsb_Multi_Rewarded_Video_contructor,
             __jsb_Multi_Rewarded_Video_class,
             jsb_Multi_Rewarded_Video_finalize)
SE_BIND_FUNC(jsb_Multi_Rewarded_Video_isLoaded)
SE_BIND_FUNC(jsb_Multi_Rewarded_Video_load)
SE_BIND_FUNC(jsb_Multi_Rewarded_Video_show)
SE_BIND_FUNC(jsb_Multi_Rewarded_Video_addItem)
SE_BIND_FUNC(jsb_Multi_Rewarded_Video_setResultCallback)

bool register_multi_rewarded_video_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    core::getOrCreatePlainObject_r("ads", core::__eeObj, &adsObj);

    auto cls = se::Class::create("MultiRewardedVideo", adsObj, nullptr,
                                 _SE(jsb_Multi_Rewarded_Video_contructor));
    cls->defineFinalizeFunction(_SE(jsb_Multi_Rewarded_Video_finalize));

    cls->defineFunction("isLoaded", _SE(jsb_Multi_Rewarded_Video_isLoaded));
    cls->defineFunction("load", _SE(jsb_Multi_Rewarded_Video_load));
    cls->defineFunction("show", _SE(jsb_Multi_Rewarded_Video_show));
    cls->defineFunction("addItem", _SE(jsb_Multi_Rewarded_Video_addItem));
    cls->defineFunction("setResultCallback",
                        _SE(jsb_Multi_Rewarded_Video_setResultCallback));

    cls->install();

    JSBClassType::registerClass<ee::MultiRewardedVideo>(cls);

    __jsb_Multi_Rewarded_Video_proto = cls->getProto();
    __jsb_Multi_Rewarded_Video_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}

} // namespace ads
} // namespace ee
