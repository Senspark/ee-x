//
//  EEUiLayoutLoader.cpp
//  ee-library
//
//  Created by Zinge on 5/10/16.
//
//

#include "EEUiLayoutLoader.hpp"

#include <ui/UILayout.h>

NS_EE_BEGIN
namespace property {
constexpr const char* background_color = "backgroundColor";
constexpr const char* background_color_opacity = "backgroundColorOpacity";
} // namespace property.

cocos2d::Node* UiLayoutLoader::createNode(cocos2d::Node* parent,
                                          cocosbuilder::CCBReader* reader) {
    auto result = cocos2d::ui::Layout::create();
    result->setBackGroundColorType(
        cocos2d::ui::Layout::BackGroundColorType::SOLID);
    return result;
}

void UiLayoutLoader::onHandlePropTypeColor3(cocos2d::Node* node,
                                            cocos2d::Node* parent,
                                            const char* propertyName,
                                            cocos2d::Color3B color3B,
                                            cocosbuilder::CCBReader* reader) {
    auto layout = dynamic_cast<cocos2d::ui::Layout*>(node);
    std::string propName(propertyName);
    if (propName == property::background_color) {
        return layout->setBackGroundColor(color3B);
    }
    Super::onHandlePropTypeColor3(node, parent, propertyName, color3B, reader);
}

void UiLayoutLoader::onHandlePropTypeByte(cocos2d::Node* node,
                                          cocos2d::Node* parent,
                                          const char* propertyName,
                                          unsigned char byte,
                                          cocosbuilder::CCBReader* reader) {
    auto layout = dynamic_cast<cocos2d::ui::Layout*>(node);
    std::string propName(propertyName);
    if (propName == property::background_color_opacity) {
        return layout->setBackGroundColorOpacity(byte);
    }
    Super::onHandlePropTypeByte(node, parent, propertyName, byte, reader);
}
NS_EE_END
