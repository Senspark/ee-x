//
//  jsb_rewarded_video.cpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#include "ee/jsb/ads/jsb_rewarded_video.hpp"

#include <unordered_map>

#include <ee/Ads.hpp>

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace ads {
se::Class* __jsb_RewardedVideo_class = nullptr;

std::unordered_map<std::shared_ptr<IRewardedVideo>, se::Object*>
    __jsb_s_rewardedVideos;
std::vector<std::shared_ptr<IRewardedVideo>> __jsb_s_rewardedVideoArchive;
} // namespace ads

namespace core {
template <>
std::shared_ptr<IRewardedVideo> get_value(const se::Value& value) {
    auto rewardedVideoPtr =
        static_cast<IRewardedVideo*>(value.toObject()->getPrivateData());
    auto iter =
        std::find_if(ads::__jsb_s_rewardedVideoArchive.cbegin(),
                     ads::__jsb_s_rewardedVideoArchive.cend(),
                     [=](const std::shared_ptr<IRewardedVideo>& ptr) -> bool {
                         return rewardedVideoPtr == ptr.get();
                     });
    if (iter != ads::__jsb_s_rewardedVideoArchive.cend()) {
        return *iter;
    } else {
        return std::shared_ptr<IRewardedVideo>(rewardedVideoPtr);
    }
}

template <>
void set_value(se::Value& value, std::shared_ptr<IRewardedVideo> input) {
    if (input != nullptr) {
        se::Object* obj = nullptr;
        if (ads::__jsb_s_rewardedVideos.count(input) != 0) {
            obj = ads::__jsb_s_rewardedVideos.at(input);
        } else {
            ads::__jsb_s_rewardedVideoArchive.push_back(input);
            obj = se::Object::createObjectWithClass(
                ads::__jsb_RewardedVideo_class);
            obj->setPrivateData(input.get());
            //        obj->root();
        }
        value.setObject(obj);
    } else {
        value.setNull();
    }
}

template <>
void set_value(se::Value& value, std::shared_ptr<IRewardedVideo>& input) {
    set_value<std::shared_ptr<IRewardedVideo>>(value, input);
}

template <>
bool jsb_finalize<IRewardedVideo>(se::State& s) {
    auto rewardedVideoPtr = static_cast<IRewardedVideo*>(s.nativeThisObject());
    auto iter =
        std::find_if(ads::__jsb_s_rewardedVideoArchive.cbegin(),
                     ads::__jsb_s_rewardedVideoArchive.cend(),
                     [=](const std::shared_ptr<IRewardedVideo>& ptr) -> bool {
                         return rewardedVideoPtr == ptr.get();
                     });
    if (iter != ads::__jsb_s_rewardedVideoArchive.cend()) {
        ads::__jsb_s_rewardedVideoArchive.erase(iter);
    } else {
        delete rewardedVideoPtr;
    }
    return true;
}
} // namespace core

namespace ads {
constexpr auto jsb_RewardedVideo_finalize = &core::jsb_finalize<IRewardedVideo>;
constexpr auto jsb_RewardedVideo_isLoaded =
    &core::jsb_accessor_get_on_ui_thread<IRewardedVideo,
                                         &IRewardedVideo::isLoaded, bool>;
constexpr auto jsb_RewardedVideo_load =
    &core::jsb_method_call_on_ui_thread<IRewardedVideo, &IRewardedVideo::load>;
constexpr auto jsb_RewardedVideo_show =
    &core::jsb_method_get_on_ui_thread<IRewardedVideo, &IRewardedVideo::show,
                                       bool>;
constexpr auto jsb_RewardedVideo_setResultCallback =
    &core::jsb_set_callback<IRewardedVideo, &IRewardedVideo::setResultCallback,
                            bool>;

SE_BIND_FINALIZE_FUNC(jsb_RewardedVideo_finalize)
SE_BIND_FUNC(jsb_RewardedVideo_isLoaded)
SE_BIND_FUNC(jsb_RewardedVideo_load)
SE_BIND_FUNC(jsb_RewardedVideo_show)
SE_BIND_FUNC(jsb_RewardedVideo_setResultCallback);

se::Class* getIRewardedVideoClass() {
    CCASSERT(__jsb_RewardedVideo_class != nullptr,
             "__jsb_RewardedVideo_class is null");
    return __jsb_RewardedVideo_class;
}

bool register_rewarded_video_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    core::getOrCreatePlainObject_r("ads", globalObj, &adsObj);

    auto cls = se::Class::create("RewardedVideo", adsObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_RewardedVideo_finalize));

    cls->defineFunction("isLoaded", _SE(jsb_RewardedVideo_isLoaded));
    cls->defineFunction("load", _SE(jsb_RewardedVideo_load));
    cls->defineFunction("show", _SE(jsb_RewardedVideo_show));
    cls->defineFunction("setResultCallback",
                        _SE(jsb_RewardedVideo_setResultCallback));
    cls->install();

    JSBClassType::registerClass<IRewardedVideo>(cls);

    __jsb_RewardedVideo_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace ads
} // namespace ee
