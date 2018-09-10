//
//  EESkeletonBoneLoader.hpp
//  ee-library
//
//  Created by Zinge on 4/11/17.
//
//

#ifndef EE_LIBRARY_SKELETON_BONE_LOADER_HPP_
#define EE_LIBRARY_SKELETON_BONE_LOADER_HPP_

#include "EENodeV3Loader.hpp"

namespace ee {
class SkeletonBoneLoader : public NodeV3Loader {
private:
    using Super = NodeV3Loader;

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SkeletonBoneLoader, loader);

protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeString(cocos2d::Node* node, cocos2d::Node* parent,
                           const char* propertyName, const char* string,
                           cocosbuilder::CCBReader* reader) override;
};
} // namespace ee

#endif /* EE_LIBRARY_SKELETON_BONE_LOADER_HPP_ */
