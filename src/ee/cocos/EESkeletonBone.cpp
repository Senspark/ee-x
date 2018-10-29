//
//  EESkeletonBone.cpp
//  ee-library
//
//  Created by Zinge on 4/11/17.
//
//

#include "EESkeletonBone.hpp"
#include "EESpineMacro.hpp"

#include <spine/SkeletonAnimation.h>

namespace ee {
using Self = SkeletonBone;

Self::SkeletonBone()
    : parentSkeleton_(nullptr) {}

bool SkeletonBone::init() {
    if (not Super::init()) {
        return false;
    }
    scheduleUpdate();
    return true;
}

void SkeletonBone::onEnter() {
    Super::onEnter();
}

void SkeletonBone::onExit() {
    Super::onExit();
}

void SkeletonBone::update(float delta) {
    Super::update(delta);

    if (parentSkeleton_ == nullptr || boneName_.empty()) {
        return;
    }

    auto bone = parentSkeleton_->findBone(boneName_);
    setPositionX(bone->worldX);
    setPositionY(bone->worldY);

#ifdef EE_SPINE_RUNTIME_V3
    setRotation(spBone_getWorldRotationX(bone));
    setScaleX(spBone_getWorldScaleX(bone));
    setScaleY(spBone_getWorldScaleY(bone));
#else  // EE_SPINE_RUNTIME_V3
    setRotation(bone->worldRotation);
    setScaleX(bone->worldScaleX);
    setScaleY(bone->worldScaleY);
#endif // EE_SPINE_RUNTIME_V3
}

void SkeletonBone::setParent(cocos2d::Node* parent) {
    Super::setParent(parent);

    if (parent == nullptr) {
        parentSkeleton_ = nullptr;
    } else {
        auto skeleton = dynamic_cast<spine::SkeletonAnimation*>(parent);
        if (skeleton != nullptr) {
            parentSkeleton_ = skeleton;
        }
    }
}

void SkeletonBone::setBoneName(const std::string& boneName) {
    boneName_ = boneName;
}
} // namespace ee
