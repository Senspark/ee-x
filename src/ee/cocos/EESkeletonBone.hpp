//
//  EESkeletonBone.hpp
//  ee-library
//
//  Created by Zinge on 4/11/17.
//
//

#ifndef EE_LIBRARY_SKELETON_BONE_HPP_
#define EE_LIBRARY_SKELETON_BONE_HPP_

#include "EECocos2dxFwd.hpp"

#include <2d/CCNode.h>

namespace ee {
class SkeletonBone : public cocos2d::Node {
private:
    using Super = cocos2d::Node;

public:
    CREATE_FUNC(SkeletonBone);

    void setBoneName(const std::string& boneName);

    virtual void setParent(cocos2d::Node* parent) override;

protected:
    SkeletonBone();
    
    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onExit() override;

    virtual void update(float delta) override;

private:
    std::string boneName_;
    spine::SkeletonAnimation* parentSkeleton_;
};
} // namespace ee

#endif /* EE_LIBRARY_SKELETON_BONE_HPP_ */
