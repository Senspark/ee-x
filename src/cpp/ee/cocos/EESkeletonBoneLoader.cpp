//
//  EESkeletonBoneLoader.cpp
//  ee-library
//
//  Created by Zinge on 4/11/17.
//
//

#include "ee/cocos/EESkeletonBoneLoader.hpp"

#ifdef EE_X_COCOS_CPP
#include "ee/cocos/EESkeletonBone.hpp"

namespace ee {
namespace property {
constexpr auto bone_name = "bone|boneList";
} // namespace property

cocos2d::Node* SkeletonBoneLoader::createNode(cocos2d::Node* parent,
                                              cocosbuilder::CCBReader* reader) {
    auto result = SkeletonBone::create();
    return result;
}

void SkeletonBoneLoader::onHandlePropTypeString(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    const char* string, cocosbuilder::CCBReader* reader) {
    auto bone = dynamic_cast<SkeletonBone*>(node);
    std::string propName{propertyName};
    if (propName == property::bone_name) {
        bone->setBoneName(string);
        return;
    }
    Super::onHandlePropTypeString(node, parent, propertyName, string, reader);
}
} // namespace ee

#endif // EE_X_COCOS_CPP