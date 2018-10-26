//
//  EESpriteWithHSVLoader.cpp
//  ee-library
//
//  Created by Zinge on 6/23/16.
//
//

#include "EESpriteWithHsvLoader.hpp"
#include "EESpriteWithHsv.hpp"

NS_EE_BEGIN
namespace property {
constexpr const char* hue = "hue";
constexpr const char* saturation = "saturation";
constexpr const char* brightness = "brightness";
constexpr const char* contrast = "contrast";
} // namespace property

cocos2d::Sprite*
SpriteWithHsvLoader::createNode(cocos2d::Node* parent,
                                cocosbuilder::CCBReader* reader) {
    return SpriteWithHsv::create();
}

void SpriteWithHsvLoader::onHandlePropTypeDegrees(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    float degrees, cocosbuilder::CCBReader* reader) {
    auto sprite = dynamic_cast<SpriteWithHsv*>(node);
    std::string prop{propertyName};
    if (prop == property::hue) {
        return sprite->setHue(degrees);
    }
    SpriteLoader::onHandlePropTypeDegrees(node, parent, propertyName, degrees,
                                          reader);
}

void SpriteWithHsvLoader::onHandlePropTypeFloat(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    float floatValue, cocosbuilder::CCBReader* reader) {
    auto sprite = dynamic_cast<SpriteWithHsv*>(node);
    std::string prop{propertyName};
    if (prop == property::saturation) {
        return sprite->setSaturation(floatValue);
    }
    if (prop == property::brightness) {
        return sprite->setBrightness(floatValue);
    }
    if (prop == property::contrast) {
        return sprite->setContrast(floatValue);
    }
    SpriteLoader::onHandlePropTypeFloat(node, parent, propertyName, floatValue,
                                        reader);
}
NS_EE_END
