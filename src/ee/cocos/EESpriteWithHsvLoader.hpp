//
//  EESpriteWithHSVLoader.hpp
//  ee-library
//
//  Created by Zinge on 6/23/16.
//
//

#ifndef EE_LIBRARY_SPRITE_WITH_HSV_LOADER_HPP_
#define EE_LIBRARY_SPRITE_WITH_HSV_LOADER_HPP_

#include "EEMacro.hpp"

#include <cocosbuilder/CCSpriteLoader.h>

NS_EE_BEGIN
class SpriteWithHsvLoader : public cocosbuilder::SpriteLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SpriteWithHsvLoader, loader);

protected:
    virtual cocos2d::Sprite*
    createNode(cocos2d::Node* parent, cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeDegrees(cocos2d::Node* node, cocos2d::Node* parent,
                            const char* propertyName, float degrees,
                            cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeFloat(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* propertyName, float floatValue,
                          cocosbuilder::CCBReader* reader) override;
};
NS_EE_END

#endif /* EE_LIBRARY_SPRITE_WITH_HSV_LOADER_HPP_ */
