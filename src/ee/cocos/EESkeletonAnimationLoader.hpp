//
//  EESkeletonAnimationLoader.hpp
//  ee-library
//
//  Created by Zinge on 5/23/16.
//
//

#ifndef EE_LIBRARY_SKELETON_ANIMATION_LOADER_HPP_
#define EE_LIBRARY_SKELETON_ANIMATION_LOADER_HPP_

#include "EENodeV3Loader.hpp"

NS_EE_BEGIN
class SkeletonAnimationLoader : public NodeV3Loader {
private:
    using Super = NodeV3Loader;

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SkeletonAnimationLoader, loader);

protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeCheck(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* propertyName, bool check,
                          cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeString(cocos2d::Node* node, cocos2d::Node* parent,
                           const char* propertyName, const char* string,
                           cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeFloat(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* propertyName, float floatValue,
                          cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeFloatScale(cocos2d::Node* node, cocos2d::Node* parent,
                               const char* propertyName, float floatScale,
                               cocosbuilder::CCBReader* reader) override;

private:
    std::string dataFile_;
    std::string atlasFile_;
    std::string animationName_;
};
NS_EE_END

#endif /* EE_LIBRARY_SKELETON_ANIMATION_LOADER_HPP_ */
