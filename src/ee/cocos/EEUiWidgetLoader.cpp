//
//  EEUiWidgetLoader.cpp
//  ee-library
//
//  Created by Zinge on 5/5/16.
//
//

#include "EEUiWidgetLoader.hpp"
#include "EEUiWidgetCallback.hpp"

#include <ui/UIWidget.h>

NS_EE_BEGIN
namespace property {
constexpr auto enabled = "enabled";
constexpr auto bright = "bright";
constexpr auto highlighted = "highlighted";
constexpr auto ignore_content_adapt_with_size = "ignoreContentAdaptWithSize";
constexpr auto size_type = "sizeType";
constexpr auto position_type = "positionType";
constexpr auto block = "block";
constexpr auto touch_enabled = "touchEnabled";
constexpr auto swallow_touches = "swallowTouches";
constexpr auto propagate_touch_events = "propagateTouchEvents";
} // namespace property.

cocos2d::Node* UiWidgetLoader::createNode(cocos2d::Node* parent,
                                          cocosbuilder::CCBReader* reader) {
    auto result = cocos2d::ui::Widget::create();
    return result;
}

void UiWidgetLoader::onHandlePropTypeBlock(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           cocosbuilder::BlockData* blockData,
                                           cocosbuilder::CCBReader* reader) {
    auto widget = dynamic_cast<cocos2d::ui::Widget*>(node);
    std::string propName(propertyName);
    if (propName == property::block) {
        if (blockData != nullptr) {
            auto callbackManager = detail::UiWidgetCallback::getInstance();
            auto&& touchCallback = callbackManager->getActiveTouchCallback();
            auto&& clickCallback = callbackManager->getActiveClickCallback();
            if (touchCallback) {
                widget->addTouchEventListener(touchCallback);
                callbackManager->setActiveTouchCallback(nullptr);
            } else if (clickCallback) {
                widget->addClickEventListener(clickCallback);
                callbackManager->setActiveClickCallback(nullptr);
            } else {
                CCASSERT(false, "Unexpected value.");
            }
        }
        return;
    }
    Super::onHandlePropTypeBlock(node, parent, propertyName, blockData, reader);
}

void UiWidgetLoader::onHandlePropTypeCheck(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName, bool check,
                                           cocosbuilder::CCBReader* reader) {
    auto widget = dynamic_cast<cocos2d::ui::Widget*>(node);
    std::string propName(propertyName);
    if (propName == property::enabled) {
        return widget->setEnabled(check);
    }
    if (propName == property::bright) {
        return widget->setBright(check);
    }
    if (propName == property::highlighted) {
        return widget->setHighlighted(check);
    }
    if (propName == property::ignore_content_adapt_with_size) {
        return widget->ignoreContentAdaptWithSize(check);
    }
    if (propName == property::touch_enabled) {
        return widget->setTouchEnabled(check);
    }
    if (propName == property::swallow_touches) {
        return widget->setSwallowTouches(check);
    }
    if (propName == property::propagate_touch_events) {
        return widget->setPropagateTouchEvents(check);
    }
    Super::onHandlePropTypeCheck(node, parent, propertyName, check, reader);
}

void UiWidgetLoader::onHandlePropTypeIntegerLabeled(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    int integerLabeled, cocosbuilder::CCBReader* reader) {
    auto widget = dynamic_cast<cocos2d::ui::Widget*>(node);
    auto parentContentSize =
        reader->getAnimationManager()->getContainerSize(parent);
    std::string propName(propertyName);
    if (propName == property::size_type) {
        auto type = static_cast<cocos2d::ui::Widget::SizeType>(integerLabeled);
        widget->setSizeType(type);
        if (type == cocos2d::ui::Widget::SizeType::PERCENT) {
            auto&& contentSize = widget->getContentSize();
            widget->setSizePercent(
                cocos2d::Vec2(contentSize.width / parentContentSize.width,
                              contentSize.height / parentContentSize.height));
        }
        return;
    }
    if (propName == property::position_type) {
        auto type =
            static_cast<cocos2d::ui::Widget::PositionType>(integerLabeled);
        widget->setPositionType(type);
        if (type == cocos2d::ui::Widget::PositionType::PERCENT) {
            auto&& position = widget->getPosition();
            widget->setPositionPercent(
                cocos2d::Vec2(position.x / parentContentSize.width,
                              position.y / parentContentSize.height));
        }
        return;
    }
    return Super::onHandlePropTypeIntegerLabeled(node, parent, propertyName,
                                                 integerLabeled, reader);
}
NS_EE_END
