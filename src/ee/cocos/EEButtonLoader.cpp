//
//  EEButtonLoader.cpp
//  ee-library
//
//  Created by Zinge on 10/24/16.
//
//

#include "EEButtonLoader.hpp"
#include "EEButton.hpp"

namespace ee {
namespace property {
constexpr auto pressed_sprite_frame_brightness = "pressedSpriteFrameBrightness";
} // namespace property

cocos2d::Node* ButtonLoader::createNode(cocos2d::Node* parent,
                                        cocosbuilder::CCBReader* reader) {
    auto result = Button::create();
    return result;
}

void ButtonLoader::onHandlePropTypeFloat(cocos2d::Node* node,
                                         cocos2d::Node* parent,
                                         const char* propertyName,
                                         float floatValue,
                                         cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<Button*>(node);
    std::string propName(propertyName);
    if (propName == property::pressed_sprite_frame_brightness) {
        return button->pressedStateBrightness(floatValue);
    }
    Super::onHandlePropTypeFloat(node, parent, propertyName, floatValue,
                                 reader);
}
} // namespace ee
