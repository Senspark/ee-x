//
//  EEUiButtonLoader.cpp
//  ee-library
//
//  Created by enrevol on 3/22/16.
//
//

#include "EEUiButtonLoader.hpp"

#include <2d/CCLabel.h>
#include <2d/CCSpriteFrameCache.h>
#include <base/CCDirector.h>
#include <renderer/CCTextureCache.h>
#include <ui/UIButton.h>

NS_EE_BEGIN
using Self = UiButtonLoader;

namespace property {
constexpr auto pressed_action_enabled = "pressedActionEnabled";
constexpr auto zoom_scale = "zoomScale";

constexpr auto title_text = "titleText";
constexpr auto title_font_name = "titleFontName";
constexpr auto title_font_size = "titleFontSize";
constexpr auto title_anchor_point = "titleAnchorPoint";
constexpr auto title_color = "titleColor";

constexpr auto scale_9_enabled = "scale9Enabled";

constexpr auto normal_sprite_frame_enabled = "normalSpriteFrameEnabled";
constexpr auto pressed_sprite_frame_enabled = "pressedSpriteFrameEnabled";
constexpr auto disabled_sprite_frame_enabled = "disabledSpriteFrameEnabled";

constexpr auto normal_sprite_frame = "normalSpriteFrame";
constexpr auto pressed_sprite_frame = "pressedSpriteFrame";
constexpr auto disabled_sprite_frame = "disabledSpriteFrame";
} // namespace property.

cocos2d::Node* UiButtonLoader::createNode(cocos2d::Node* parent,
                                          cocosbuilder::CCBReader* reader) {
    auto result = cocos2d::ui::Button::create();
    return result;
}

Self::UiButtonLoader()
    : spriteFrameEnabled_(false)
    , textureResType_(cocos2d::ui::Widget::TextureResType::LOCAL) {}

cocos2d::SpriteFrame* UiButtonLoader::parsePropTypeSpriteFrame(
    cocos2d::Node* node, cocos2d::Node* parent, cocosbuilder::CCBReader* reader,
    const char* propertyName) {
    auto spriteSheet = reader->readCachedString();
    auto spriteFile = reader->readCachedString();
    cocos2d::SpriteFrame* spriteFrame = nullptr;
    if (spriteFrameEnabled_ && spriteFile.empty() == false) {
        if (spriteSheet.empty()) {
            spriteFile = reader->getCCBRootPath() + spriteFile;
            spriteFrameName_ = spriteFile;
            textureResType_ = cocos2d::ui::Widget::TextureResType::LOCAL;
            auto texture =
                cocos2d::Director::getInstance()->getTextureCache()->addImage(
                    spriteFile.c_str());
            if (texture != nullptr) {
                auto bounds =
                    cocos2d::Rect(0, 0, texture->getContentSize().width,
                                  texture->getContentSize().height);
                spriteFrame =
                    cocos2d::SpriteFrame::createWithTexture(texture, bounds);
            }
        } else {
            auto frameCache = cocos2d::SpriteFrameCache::getInstance();
            spriteSheet = reader->getCCBRootPath() + spriteSheet;
            // Load the sprite sheet only if it is not loaded.
            if (reader->getLoadedSpriteSheet().find(spriteSheet) ==
                reader->getLoadedSpriteSheet().end()) {
                frameCache->addSpriteFramesWithFile(spriteSheet.c_str());
                reader->getLoadedSpriteSheet().insert(spriteSheet);
            }
            spriteFrame = frameCache->getSpriteFrameByName(spriteFile.c_str());
            spriteFrameName_ = spriteFile;
            textureResType_ = cocos2d::ui::Widget::TextureResType::PLIST;
        }
        if (reader->getAnimatedProperties()->find(propertyName) !=
            reader->getAnimatedProperties()->end()) {
            reader->getAnimationManager()->setObject(spriteFrame, node,
                                                     propertyName);
        }
    }
    return spriteFrame;
}

void UiButtonLoader::onHandlePropTypePoint(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           cocos2d::Point point,
                                           cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    std::string propName(propertyName);
    if (propName == property::title_anchor_point) {
        return button->getTitleRenderer()->setAnchorPoint(point);
    }
    return Super::onHandlePropTypePoint(node, parent, propertyName, point,
                                        reader);
}

void UiButtonLoader::onHandlePropTypeCheck(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName, bool check,
                                           cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    std::string propName(propertyName);
    if (propName == property::pressed_action_enabled) {
        return button->setPressedActionEnabled(check);
    }
    if (propName == property::scale_9_enabled) {
        return button->setScale9Enabled(check);
    }
    if (propName == property::normal_sprite_frame_enabled ||
        propName == property::pressed_sprite_frame_enabled ||
        propName == property::disabled_sprite_frame_enabled) {
        spriteFrameEnabled_ = check;
        return;
    }
    Super::onHandlePropTypeCheck(node, parent, propertyName, check, reader);
}

void UiButtonLoader::onHandlePropTypeString(cocos2d::Node* node,
                                            cocos2d::Node* parent,
                                            const char* propertyName,
                                            const char* string,
                                            cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    std::string propName(propertyName);
    if (propName == property::title_text) {
        return button->setTitleText(string);
    }
    Super::onHandlePropTypeString(node, parent, propertyName, string, reader);
}

void UiButtonLoader::onHandlePropTypeFontTTF(cocos2d::Node* node,
                                             cocos2d::Node* parent,
                                             const char* propertyName,
                                             const char* fontTTF,
                                             cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    std::string propName(propertyName);
    if (propName == property::title_font_name) {
        return button->setTitleFontName(fontTTF);
    }
    Super::onHandlePropTypeFontTTF(node, parent, propertyName, fontTTF, reader);
}

void UiButtonLoader::onHandlePropTypeFloat(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           float floatValue,
                                           cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    std::string propName(propertyName);
    if (propName == property::zoom_scale) {
        return button->setZoomScale(floatValue);
    }
    Super::onHandlePropTypeFloat(node, parent, propertyName, floatValue,
                                 reader);
}

void UiButtonLoader::onHandlePropTypeFloatScale(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    float floatScale, cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    std::string propName(propertyName);
    if (propName == property::title_font_size) {
        return button->setTitleFontSize(floatScale);
    }
    Super::onHandlePropTypeFloatScale(node, parent, propertyName, floatScale,
                                      reader);
}

void UiButtonLoader::onHandlePropTypeSpriteFrame(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    cocos2d::SpriteFrame* spriteFrame, cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    std::string propName(propertyName);
    if (spriteFrameEnabled_) {
        CCASSERT(spriteFrameName_.empty() == false,
                 "Sprite frame name is empty!");
    }
    if (propName == property::normal_sprite_frame) {
        if (spriteFrameEnabled_) {
            button->loadTextureNormal(spriteFrameName_, textureResType_);
        }
        return;
    }
    if (propName == property::pressed_sprite_frame) {
        if (spriteFrameEnabled_) {
            button->loadTexturePressed(spriteFrameName_, textureResType_);
        }
        return;
    }
    if (propName == property::disabled_sprite_frame) {
        if (spriteFrameEnabled_) {
            button->loadTextureDisabled(spriteFrameName_, textureResType_);
        }
        return;
    }
    if (spriteFrameEnabled_) {
        CCASSERT(false, "Sprite frame is not loaded!");
    }
    Super::onHandlePropTypeSpriteFrame(node, parent, propertyName, spriteFrame,
                                       reader);
}

void UiButtonLoader::onHandlePropTypeColor3(cocos2d::Node* node,
                                            cocos2d::Node* parent,
                                            const char* propertyName,
                                            cocos2d::Color3B color3B,
                                            cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::Button*>(node);
    std::string propName(propertyName);
    if (propName == property::title_color) {
        return button->setTitleColor(color3B);
    }
    Super::onHandlePropTypeColor3(node, parent, propertyName, color3B, reader);
}
NS_EE_END
