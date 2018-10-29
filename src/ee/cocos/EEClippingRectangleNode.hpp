//
//  EEClippingRectangleNode.hpp
//  ee-library
//
//  Created by Zinge on 9/1/17.
//
//

#ifndef EE_LIBRARY_CLIPPING_RECTANGLE_NODE_HPP
#define EE_LIBRARY_CLIPPING_RECTANGLE_NODE_HPP

#include <2d/CCClippingRectangleNode.h>
#include <renderer/CCCustomCommand.h>

namespace ee {
class ClippingRectangleNode : public cocos2d::ClippingRectangleNode {
private:
    using Self = ClippingRectangleNode;
    using Super = cocos2d::ClippingRectangleNode;

public:
    static Self* create();

    static Self* create(const cocos2d::Rect& region);

    ClippingRectangleNode();
    virtual ~ClippingRectangleNode() override;

    virtual void visit(cocos2d::Renderer* renderer,
                       const cocos2d::Mat4& parentTransform,
                       std::uint32_t parentFlags) override;

protected:
    void onBeforeVisitScissor();
    void onAfterVisitScissor();

    bool hasPreviousScissorRect_;
    cocos2d::Rect previousScissorRect_;
};
} // namespace ee

#endif /* EE_LIBRARY_CLIPPING_RECTANGLE_NODE_HPP */
