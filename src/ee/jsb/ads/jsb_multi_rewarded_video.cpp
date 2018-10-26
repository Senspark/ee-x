//
//  jsb_multi_rewarded_video.cpp
//  ee-x
//
//  Created by Kiet Le on 9/04/18.
//

#include "ee/jsb/ads/jsb_multi_rewarded_video.hpp"

#include <unordered_map>

#include <ee/Ads.hpp>

#include "ee/jsb/ads/jsb_rewarded_video.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace ads {

se::Class* __jsb_Multi_Rewarded_Video_class = nullptr;

constexpr auto jsb_Multi_Rewarded_Video_finalize =
    &core::jsb_finalize<MultiRewardedVideo>;
constexpr auto jsb_Multi_Rewarded_Video_contructor =
    &core::jsb_constructor<MultiRewardedVideo>;
constexpr auto jsb_Multi_Rewarded_Video_addItem =
    &core::jsb_method_call_on_ui_thread<MultiRewardedVideo,
                                        &MultiRewardedVideo::addItem,
                                        std::shared_ptr<IRewardedVideo>>;

SE_BIND_FINALIZE_FUNC(jsb_Multi_Rewarded_Video_finalize)
SE_BIND_CTOR(jsb_Multi_Rewarded_Video_contructor,
             __jsb_Multi_Rewarded_Video_class,
             jsb_Multi_Rewarded_Video_finalize)
SE_BIND_FUNC(jsb_Multi_Rewarded_Video_addItem)

bool register_multi_rewarded_video_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    se::Object* eeObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("ads", eeObj, &adsObj);

    auto cls = se::Class::create("MultiRewardedVideo", adsObj,
                                 getIRewardedVideoClass()->getProto(),
                                 _SE(jsb_Multi_Rewarded_Video_contructor));
    cls->defineFinalizeFunction(_SE(jsb_Multi_Rewarded_Video_finalize));

    cls->defineFunction("addItem", _SE(jsb_Multi_Rewarded_Video_addItem));

    cls->install();

    JSBClassType::registerClass<MultiRewardedVideo>(cls);

    __jsb_Multi_Rewarded_Video_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}

} // namespace ads
} // namespace ee
