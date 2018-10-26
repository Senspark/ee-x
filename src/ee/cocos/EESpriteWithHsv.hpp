//
//  EESpriteWithHSV.hpp
//  ee-library
//
//  Created by Zinge on 6/22/16.
//
//

#ifndef EE_LIBRARY_SPRITE_WITH_HSV_HPP_
#define EE_LIBRARY_SPRITE_WITH_HSV_HPP_

#include "EEHsvProtocol.hpp"

#include <2d/CCSprite.h>

namespace ee {
class SpriteWithHsvLoader;

class SpriteWithHsv : public cocos2d::Sprite, public HsvProtocol {
private:
    using Self = SpriteWithHsv;
    using Super = cocos2d::Sprite;

public:
    using Loader = SpriteWithHsvLoader;

    static Self* create();

    static Self* create(const std::string& filename);

    static Self* create(const std::string& filename, const cocos2d::Rect& rect);

    static Self* createWithTexture(cocos2d::Texture2D* texture);

    static Self* createWithTexture(cocos2d::Texture2D* texture,
                                   const cocos2d::Rect& rect,
                                   bool rotated = false);

    static Self* createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame);

    static Self* createWithSpriteFrameName(const std::string& spriteFrameName);
    
    virtual void
    setGLProgramState(cocos2d::GLProgramState* glProgramState) override;

protected:
    using Super::initWithTexture;

    template <class T, class Initializer>
    friend T* createInstance(const Initializer& initializer);

    SpriteWithHsv();

    virtual bool initWithTexture(cocos2d::Texture2D* texture,
                                 const cocos2d::Rect& rect,
                                 bool rotated) override;

    virtual void draw(cocos2d::Renderer* renderer,
                      const cocos2d::Mat4& transform,
                      std::uint32_t flags) override;

    virtual bool updateMatrix() override;

    void initShader();
    
private:
    cocos2d::GLProgramState* createState();
    
    cocos2d::GLProgramState* customState_;
};
} // namespace ee

#endif /* EE_LIBRARY_SPRITE_WITH_HSV_HPP_ */
