//
//  EEBlurBackground.hpp
//  ee-library
//
//  Created by Zinge on 6/24/16.
//
//

#ifndef EE_LIBRARY_BLUR_BACKGROUND_HPP_
#define EE_LIBRARY_BLUR_BACKGROUND_HPP_

#include "EECocos2dxFwd.hpp"

#include <2d/CCNode.h>

namespace ee {
class BlurBackground : public cocos2d::Node {
private:
    using Self = BlurBackground;
    using Super = cocos2d::Node;

public:
    CREATE_FUNC(Self);

    /// Sets the render scale.
    /// Smaller value offers better performace but more blurry.
    /// Default is 1.0f.
    /// @param[in] scale Render scale.
    Self* setRenderScale(float scale);

    /// Sets the blur strength.
    /// Default is 2.0f.
    /// @param[in] sigma
    Self* setSigma(float sigma);

    /// Default is 4.
    Self* setBlurRadius(std::size_t radius);

    /// Default is false.
    Self* setLinearSamplingEnabled(bool enabled);

    /// Sets whether to dynamically blur the screen.
    /// Default is true.
    Self* setAutoRender(bool enabled);

    /// Updates the blurred screen immediately.
    Self* render();

protected:
    virtual bool init() override;

    virtual void update(float delta) override;

    virtual void visit(cocos2d::Renderer* renderer,
                       const cocos2d::Mat4& parentTransforms,
                       std::uint32_t parentFlags) override;

    void updateRenderers();

    void createRenderers();

    void resetRenderers();

    void configRenderer(cocos2d::RenderTexture* renderer);

    void configHorizontalRenderer(cocos2d::RenderTexture* renderer);

    void configVerticalRenderer(cocos2d::RenderTexture* renderer);

    float renderScale_;
    float sigma_;
    std::size_t blurRadius_;
    bool useLinearSampling_;

    bool rendererInitialized_;
    bool rendererDirty_;

    cocos2d::Size renderSize_;
    cocos2d::RenderTexture* verticalRenderer_;
    cocos2d::RenderTexture* horizontalRenderer_;
};
} // namespace ee

#endif /* EE_LIBRARY_BLUR_BACKGROUND_HPP_ */
