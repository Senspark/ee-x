//
//  jsb_rewarded_video.cpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#include "jsb_rewarded_video.hpp"

#include "jsb_core_common.hpp"

#include "ee/jsb/core/jsb_templates.hpp"

#include <ee/Ads.hpp>

#include <unordered_map>

namespace ee {
namespace ads {
static se::Object* __jsb_RewardedVideo_proto = nullptr;
static se::Class* __jsb_RewardedVideo_class = nullptr;

static std::unordered_map<std::shared_ptr<ee::IRewardedVideo>, se::Object*>
    __jsb_s_rewardedVideos;
static std::vector<std::shared_ptr<ee::IRewardedVideo>>
    __jsb_s_rewardedVideoArchive;
} // namespace ads

namespace core {
void set_value(se::Value& value, std::shared_ptr<ee::IRewardedVideo> input) {
    se::Object* obj = nullptr;
    if (ads::__jsb_s_rewardedVideos.count(input) != 0) {
        obj = ads::__jsb_s_rewardedVideos.at(input);
    } else {
        ads::__jsb_s_rewardedVideoArchive.push_back(input);
        obj = se::Object::createObjectWithClass(ads::__jsb_RewardedVideo_class);
        obj->setPrivateData(input.get());
        obj->root();
    }
    value.setObject(obj);
}
} // namespace core

namespace ads {
constexpr auto jsb_RewardedVideo_finalize =
    &ee::core::jsb_finalize<IRewardedVideo>;
constexpr auto jsb_RewardedVideo_isLoaded =
    &ee::core::jsb_accessor_get_on_ui_thread<IRewardedVideo,
                                             &IRewardedVideo::isLoaded, bool>;
constexpr auto jsb_RewardedVideo_load =
    &ee::core::jsb_method_call_on_ui_thread<IRewardedVideo,
                                            &IRewardedVideo::load>;
constexpr auto jsb_RewardedVideo_show =
    &ee::core::jsb_method_call_on_ui_thread<IRewardedVideo,
                                            &IRewardedVideo::show>;

SE_BIND_FINALIZE_FUNC(jsb_RewardedVideo_finalize)
SE_BIND_FUNC(jsb_RewardedVideo_isLoaded)
SE_BIND_FUNC(jsb_RewardedVideo_load)
SE_BIND_FUNC(jsb_RewardedVideo_show)

bool register_rewarded_video_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    core::getOrCreatePlainObject_r("ads", globalObj, &adsObj);

    auto cls = se::Class::create("InterstitialAd", adsObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_RewardedVideo_finalize));

    cls->defineFunction("isLoaded", _SE(jsb_RewardedVideo_isLoaded));
    cls->defineFunction("load", _SE(jsb_RewardedVideo_load));
    cls->defineFunction("show", _SE(jsb_RewardedVideo_show));

    cls->install();

    JSBClassType::registerClass<ee::IInterstitialAd>(cls);

    __jsb_RewardedVideo_proto = cls->getProto();
    __jsb_RewardedVideo_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace ads

} // namespace ee
