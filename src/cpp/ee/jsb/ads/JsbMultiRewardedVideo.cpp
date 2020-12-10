//
//  jsb_multi_rewarded_video.cpp
//  ee-x
//
//  Created by Kiet Le on 9/04/18.
//

#include "ee/jsb/ads/JsbMultiRewardedVideo.hpp"

#include <ee/ads/MultiRewardedVideo.hpp>
#include <ee/jsb/JsbCore.hpp>

#include "ee/jsb/ads/JsbIRewardedVideo.hpp"

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, MultiRewardedVideo& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core

namespace ads {
namespace {
se::Class* clazz = nullptr;

using Self = MultiRewardedVideo;

// clang-format off
constexpr auto finalize    = &core::makeFinalize<Self>;
constexpr auto constructor = &core::makeConstructor<Self>;
constexpr auto addItem     = &core::makeInstanceMethod<&Self::addItem>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(addItem)
} // namespace

bool registerJsbMultiRewardedVideo(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto parent = getIRewardedVideoClass()->getProto();
    auto cls = se::Class::create("MultiRewardedVideo", scope, parent,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, addItem);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace ads
} // namespace ee
