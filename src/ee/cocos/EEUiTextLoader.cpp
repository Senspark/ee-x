//
//  EEUiTextLoader.cpp
//  ee-library
//
//  Created by enrevol on 3/25/16.
//
//

#include "EEUiTextLoader.hpp"

#include <ui/UIText.h>

NS_EE_BEGIN
using Self = UiTextLoader;

namespace property {
constexpr const char* font_name = "fontName";
constexpr const char* font_size = "fontSize";
constexpr const char* text_color = "textColor";
constexpr const char* text_area_size = "textAreaSize";
constexpr const char* text_horizontal_alignment = "textHorizontalAlignment";
constexpr const char* text_vertical_alignment = "textVerticalAlignment";
constexpr const char* touch_scale_change_enabled = "touchScaleChangeEnabled";
constexpr const char* string = "string";

constexpr const char* shadow_enabled = "shadowEnabled";
constexpr const char* shadow_opacity = "shadowOpacity";
constexpr const char* shadow_color = "shadowColor";
constexpr const char* shadow_offset = "shadowOffset";
constexpr const char* shadow_blur_radius = "shadowBlurRadius";

constexpr const char* outline_enabled = "outlineEnabled";
constexpr const char* outline_opacity = "outlineOpacity";
constexpr const char* outline_color = "outlineColor";
constexpr const char* outline_size = "outlineSize";
} // namespace property.

cocos2d::Node* UiTextLoader::createNode(cocos2d::Node* parent,
                                        cocosbuilder::CCBReader* reader) {
    return cocos2d::ui::Text::create();
}

Self::UiTextLoader()
    : shadowEnabled_(false)
    , shadowOpacity_(0)
    , shadowBlurRadius_(0)
    , outlineEnabled_(false)
    , outlineOpacity_(0)
    , outlineSize_(0) {}

void UiTextLoader::onHandlePropTypePoint(cocos2d::Node* node,
                                         cocos2d::Node* parent,
                                         const char* propertyName,
                                         cocos2d::Vec2 point,
                                         cocosbuilder::CCBReader* reader) {
    std::string propName(propertyName);
    if (propName == property::shadow_offset) {
        shadowOffset_ = point;
        return;
    }
    Super::onHandlePropTypePoint(node, parent, propertyName, point, reader);
}

void UiTextLoader::onHandlePropTypeCheck(cocos2d::Node* node,
                                         cocos2d::Node* parent,
                                         const char* propertyName, bool check,
                                         cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    std::string propName(propertyName);
    if (propName == property::touch_scale_change_enabled) {
        return text->setTouchScaleChangeEnabled(check);
    }
    if (propName == property::shadow_enabled) {
        shadowEnabled_ = check;
        return;
    }
    if (propName == property::outline_enabled) {
        outlineEnabled_ = check;
        return;
    }
    Super::onHandlePropTypeCheck(node, parent, propertyName, check, reader);
}

void UiTextLoader::onHandlePropTypeColor3(cocos2d::Node* node,
                                          cocos2d::Node* parent,
                                          const char* propertyName,
                                          cocos2d::Color3B color3B,
                                          cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    std::string propName(propertyName);
    if (propName == property::text_color) {
        return text->setTextColor(cocos2d::Color4B(color3B));
    }
    if (propName == property::shadow_color) {
        shadowColor_ = color3B;
        return;
    }
    if (propName == property::outline_color) {
        outlineColor_ = color3B;
        return;
    }
    Super::onHandlePropTypeColor3(node, parent, propertyName, color3B, reader);
}

void UiTextLoader::onHandlePropTypeByte(cocos2d::Node* node,
                                        cocos2d::Node* parent,
                                        const char* propertyName,
                                        unsigned char byte,
                                        cocosbuilder::CCBReader* reader) {
    std::string propName(propertyName);
    if (propName == property::shadow_opacity) {
        shadowOpacity_ = byte;
        return;
    }
    if (propName == property::outline_opacity) {
        outlineOpacity_ = byte;
        return;
    }
    Super::onHandlePropTypeByte(node, parent, propertyName, byte, reader);
}

void UiTextLoader::onHandlePropTypeFontTTF(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           const char* fontTTF,
                                           cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    std::string propName(propertyName);
    if (propName == property::font_name) {
        return text->setFontName(fontTTF);
    }
    Super::onHandlePropTypeFontTTF(node, parent, propertyName, fontTTF, reader);
}

void UiTextLoader::onHandlePropTypeText(cocos2d::Node* node,
                                        cocos2d::Node* parent,
                                        const char* propertyName,
                                        const char* text,
                                        cocosbuilder::CCBReader* reader) {
    auto uiText = dynamic_cast<cocos2d::ui::Text*>(node);
    std::string propName(propertyName);
    if (propName == property::string) {
        return uiText->setString(text);
    }
    Super::onHandlePropTypeText(node, parent, propertyName, text, reader);
}

void UiTextLoader::onHandlePropTypeFloatScale(cocos2d::Node* node,
                                              cocos2d::Node* parent,
                                              const char* propertyName,
                                              float floatScale,
                                              cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    std::string propName(propertyName);
    if (propName == property::font_size) {
        return text->setFontSize(floatScale);
    }
    Super::onHandlePropTypeFloatScale(node, parent, propertyName, floatScale,
                                      reader);
}

void UiTextLoader::onHandlePropTypeInteger(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           int integer,
                                           cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    std::string propName(propertyName);
    if (propName == property::shadow_blur_radius) {
        shadowBlurRadius_ = integer;
        if (shadowEnabled_) {
            cocos2d::Color4B color(shadowColor_);
            color.a = shadowOpacity_;
            text->enableShadow(color, shadowOffset_, shadowBlurRadius_);
        }
        return;
    }
    if (propName == property::outline_size) {
        outlineSize_ = integer;
        if (outlineEnabled_) {
            cocos2d::Color4B color(outlineColor_);
            color.a = outlineOpacity_;
            text->enableOutline(color, outlineSize_);
        }
        return;
    }
    Super::onHandlePropTypeInteger(node, parent, propertyName, integer, reader);
}

void UiTextLoader::onHandlePropTypeIntegerLabeled(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    int integerLabeled, cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    std::string propName(propertyName);
    if (propName == property::text_horizontal_alignment) {
        auto alignment = static_cast<cocos2d::TextHAlignment>(integerLabeled);
        return text->setTextHorizontalAlignment(alignment);
    }
    if (propName == property::text_vertical_alignment) {
        auto alignment = static_cast<cocos2d::TextVAlignment>(integerLabeled);
        return text->setTextVerticalAlignment(alignment);
    }
    Super::onHandlePropTypeIntegerLabeled(node, parent, propertyName,
                                          integerLabeled, reader);
}

void UiTextLoader::onHandlePropTypeSize(cocos2d::Node* node,
                                        cocos2d::Node* parent,
                                        const char* propertyName,
                                        cocos2d::Size size,
                                        cocosbuilder::CCBReader* reader) {
    auto text = dynamic_cast<cocos2d::ui::Text*>(node);
    std::string propName(propertyName);
    if (propName == property::text_area_size) {
        return text->setTextAreaSize(size);
    }
    Super::onHandlePropTypeSize(node, parent, propertyName, size, reader);
}
NS_EE_END
