//
//  EEUiCheckBoxLoader.cpp
//  ee-library
//
//  Created by Zinge on 5/27/16.
//
//

#include "EEUiCheckBoxLoader.hpp"

#include <2d/CCSpriteFrameCache.h>
#include <base/CCDirector.h>
#include <renderer/CCTextureCache.h>
#include <ui/UICheckBox.h>

NS_EE_BEGIN
using Self = UiCheckBoxLoader;

namespace property {
constexpr const char* selected = "selected";
constexpr const char* zoom_scale = "zoomScale";

constexpr const char* normal_cross_sprite_frame_enabled =
    "normalCrossSpriteFrameEnabled";
constexpr const char* disabled_cross_sprite_frame_enabled =
    "disabledCrossSpriteFrameEnabled";

constexpr const char* normal_cross_sprite_frame = "normalCrossSpriteFrame";
constexpr const char* disabled_cross_sprite_frame = "disabledCrossSpriteFrame";

constexpr const char* normal_background_sprite_frame_enabled =
    "normalBackgroundSpriteFrameEnabled";
constexpr const char* pressed_background_sprite_frame_enabled =
    "pressedBackgroundSpriteFrameEnabled";
constexpr const char* disabled_background_sprite_frame_enabled =
    "disabledBackgroundSpriteFrameEnabled";

constexpr const char* normal_background_sprite_frame =
    "normalBackgroundSpriteFrame";
constexpr const char* pressed_background_sprite_frame =
    "pressedBackgroundSpriteFrame";
constexpr const char* disabled_background_sprite_frame =
    "disabledBackgroundSpriteFrame";
} // namespace property.

Self::UiCheckBoxLoader()
    : spriteFrameEnabled_(false)
    , textureResType_(cocos2d::ui::Widget::TextureResType::LOCAL) {}

cocos2d::Node* UiCheckBoxLoader::createNode(cocos2d::Node* parent,
                                            cocosbuilder::CCBReader* reader) {
    auto result = cocos2d::ui::CheckBox::create();
    return result;
}

cocos2d::SpriteFrame* UiCheckBoxLoader::parsePropTypeSpriteFrame(
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

void UiCheckBoxLoader::onHandlePropTypeCheck(cocos2d::Node* node,
                                             cocos2d::Node* parent,
                                             const char* propertyName,
                                             bool check,
                                             cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::CheckBox*>(node);
    std::string propName(propertyName);
    if (propName == property::selected) {
        return button->setSelected(check);
    }
    if (propName == property::normal_cross_sprite_frame_enabled ||
        propName == property::disabled_cross_sprite_frame_enabled ||
        propName == property::normal_background_sprite_frame_enabled ||
        propName == property::pressed_background_sprite_frame_enabled ||
        propName == property::disabled_background_sprite_frame_enabled) {
        spriteFrameEnabled_ = check;
        return;
    }
    Super::onHandlePropTypeCheck(node, parent, propertyName, check, reader);
}

void UiCheckBoxLoader::onHandlePropTypeFloat(cocos2d::Node* node,
                                             cocos2d::Node* parent,
                                             const char* propertyName,
                                             float floatValue,
                                             cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::CheckBox*>(node);
    std::string propName(propertyName);
    if (propName == property::zoom_scale) {
        return button->setZoomScale(floatValue);
    }
    Super::onHandlePropTypeFloat(node, parent, propertyName, floatValue,
                                 reader);
}

void UiCheckBoxLoader::onHandlePropTypeSpriteFrame(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    cocos2d::SpriteFrame* spriteFrame, cocosbuilder::CCBReader* reader) {
    auto button = dynamic_cast<cocos2d::ui::CheckBox*>(node);
    std::string propName(propertyName);
    if (spriteFrameEnabled_) {
        CCASSERT(spriteFrameName_.empty() == false,
                 "Sprite frame name is empty!");
    }
    if (propName == property::normal_cross_sprite_frame) {
        if (spriteFrameEnabled_) {
            button->loadTextureFrontCross(spriteFrameName_, textureResType_);
        }
        return;
    }
    if (propName == property::disabled_cross_sprite_frame) {
        if (spriteFrameEnabled_) {
            button->loadTextureFrontCrossDisabled(spriteFrameName_,
                                                  textureResType_);
        }
        return;
    }
    if (propName == property::normal_background_sprite_frame) {
        if (spriteFrameEnabled_) {
            button->loadTextureBackGround(spriteFrameName_, textureResType_);
        }
        return;
    }
    if (propName == property::pressed_background_sprite_frame) {
        if (spriteFrameEnabled_) {
            button->loadTextureBackGroundSelected(spriteFrameName_,
                                                  textureResType_);
        }
        return;
    }
    if (propName == property::disabled_background_sprite_frame) {
        if (spriteFrameEnabled_) {
            button->loadTextureBackGroundDisabled(spriteFrameName_,
                                                  textureResType_);
        }
        return;
    }
    if (spriteFrameEnabled_) {
        CCASSERT(false, "Sprite frame is not loaded!");
    }
    Super::onHandlePropTypeSpriteFrame(node, parent, propertyName, spriteFrame,
                                       reader);
}
NS_EE_END
