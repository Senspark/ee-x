//
//  EESkeletonAnimationLoader.cpp
//  ee-library
//
//  Created by Zinge on 5/23/16.
//
//

#include "EESkeletonAnimationLoader.hpp"
#include "EESpineFactory.hpp"
#include "EESpineMacro.hpp"

#include <spine/SkeletonAnimation.h>

NS_EE_BEGIN
namespace property {
constexpr const char* data_file = "dataFile";
constexpr const char* atlas_file = "atlasFile";
constexpr const char* animation_scale = "animationScale";
constexpr const char* animation_name = "animation|animationList";
constexpr const char* skin_name = "skin|skinList";
constexpr const char* loop = "loop";
constexpr const char* time_scale = "timeScale";
} // namespace property

cocos2d::Node*
SkeletonAnimationLoader::createNode(cocos2d::Node* parent,
                                    cocosbuilder::CCBReader* reader) {
    auto result = new spine::SkeletonAnimation();
    result->autorelease();
    return result;
}

void SkeletonAnimationLoader::onHandlePropTypeCheck(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    bool check, cocosbuilder::CCBReader* reader) {
    auto skeleton = dynamic_cast<spine::SkeletonAnimation*>(node);
    std::string propName{propertyName};
    if (propName == property::loop) {
        if (animationName_ != "None") {
            skeleton->setAnimation(0, animationName_, check);
        }
        return;
    }
    Super::onHandlePropTypeCheck(node, parent, propertyName, check, reader);
}

void SkeletonAnimationLoader::onHandlePropTypeString(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    const char* string, cocosbuilder::CCBReader* reader) {
    auto skeleton = dynamic_cast<spine::SkeletonAnimation*>(node);
    std::string propName{propertyName};
    if (propName == property::data_file) {
        dataFile_ = string;
        return;
    }
    if (propName == property::atlas_file) {
        atlasFile_ = string;
        return;
    }
    if (propName == property::animation_name) {
        animationName_ = string;
        return;
    }
    if (propName == property::skin_name) {
        skeleton->setSkin(string);
        return;
    }
    Super::onHandlePropTypeString(node, parent, propertyName, string, reader);
}

void SkeletonAnimationLoader::onHandlePropTypeFloat(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    float floatValue, cocosbuilder::CCBReader* reader) {
    auto skeleton = dynamic_cast<spine::SkeletonAnimation*>(node);
    std::string propName{propertyName};
    if (propName == property::time_scale) {
        return skeleton->setTimeScale(floatValue);
    }
    Super::onHandlePropTypeFloat(node, parent, propertyName, floatValue,
                                 reader);
}

void SkeletonAnimationLoader::onHandlePropTypeFloatScale(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    float floatScale, cocosbuilder::CCBReader* reader) {
    auto skeleton = dynamic_cast<spine::SkeletonAnimation*>(node);
    std::string propName{propertyName};
    if (propName == property::animation_scale) {
        auto&& data = SpineFactory::getInstance()->getSkeletonData(
            dataFile_, atlasFile_, floatScale);
        skeleton->initWithData(data);
#ifdef EE_SPINE_RUNTIME_V2
        // Initialize in v2 is not virtual.
        // We should manually call it.
        skeleton->initialize();
#endif // EE_SPINE_RUNTIME_V2
        return;
    }
    Super::onHandlePropTypeFloatScale(node, parent, propertyName, floatScale,
                                      reader);
}

NS_EE_END
