//
//  EEWidget.cpp
//  ee-library
//
//  Created by eps on 6/4/18.
//

#include "EEUtils.hpp"
#include "EEWidget.hpp"

#ifndef NDEBUG
#include <2d/CCDrawNode.h>
#endif // NDEBUG

namespace ee {
namespace ui {
using Self = Widget;

Self* Self::create() {
    return createInstance(&Self::init);
}

bool Self::init() {
    if (not Super::init()) {
        return false;
    }
    magicEnabled_ = false;

#ifndef NDEBUG
    drawNode_ = cocos2d::DrawNode::create();
    drawNode_->setVisible(false);
    addProtectedChild(drawNode_, std::numeric_limits<int>::min());
#endif // NDEBUG

    container_ = cocos2d::ui::Widget::create();
    container_->ignoreContentAdaptWithSize(false);
    container_->setSwallowTouches(false);
    container_->setPropagateTouchEvents(true);
    container_->setFocusEnabled(false);
    container_->setPositionType(PositionType::ABSOLUTE);
    container_->setPosition(cocos2d::Point::ZERO);
    container_->setSizeType(SizeType::ABSOLUTE);
    container_->setContentSize(cocos2d::Size::ZERO);
    container_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    addProtectedChild(container_, std::numeric_limits<int>::min());
    return true;
}

void Self::onEnter() {
    Super::onEnter();
}

void Self::onExit() {
    Super::onExit();
}

void Self::onNodeLoaded(cocos2d::Node* node,
                        cocosbuilder::NodeLoader* nodeLoader) {
    CC_ASSERT(magicEnabled_);
    magicEnabled_ = false;
}

void Self::addChild(cocos2d::Node* child, int localZOrder, int tag) {
    container_->addChild(child, localZOrder, tag);
}

void Self::addChild(cocos2d::Node* child, int localZOrder,
                    const std::string& name) {
    container_->addChild(child, localZOrder, name);
}

cocos2d::Node* Self::getChildByTag(int tag) const {
    return container_->getChildByTag(tag);
}

cocos2d::Node* Self::getChildByName(const std::string& name) const {
    return container_->getChildByName(name);
}

cocos2d::Vector<cocos2d::Node*>& Self::getChildren() {
    return container_->getChildren();
}

const cocos2d::Vector<cocos2d::Node*>& Self::getChildren() const {
    return container_->getChildren();
}

ssize_t Self::getChildrenCount() const {
    return container_->getChildrenCount();
}

void Self::removeChild(cocos2d::Node* child, bool cleanup) {
    container_->removeChild(child, cleanup);
}

void Self::removeAllChildrenWithCleanup(bool cleanup) {
    container_->removeAllChildrenWithCleanup(cleanup);
}

void Self::reorderChild(cocos2d::Node* child, int localZOrder) {
    if (child == container_) {
        Super::reorderChild(child, localZOrder);
    } else {
        // Forward to the container.
        container_->reorderChild(child, localZOrder);
    }
}

void Self::sortAllChildren() {
    container_->sortAllChildren();
}

void Self::setCascadeColorEnabled(bool enabled) {
    Super::setCascadeColorEnabled(enabled);
    container_->setCascadeColorEnabled(enabled);
}

void Self::setCascadeOpacityEnabled(bool enabled) {
    Super::setCascadeOpacityEnabled(enabled);
    container_->setCascadeOpacityEnabled(enabled);
}

const cocos2d::Size& Self::getContentSize() const {
    if (magicEnabled_) {
        return container_->getContentSize();
    }
    return Super::getContentSize();
}

void Self::setInset(float inset) {
    setInsetLeft(inset);
    setInsetTop(inset);
    setInsetRight(inset);
    setInsetBottom(inset);
}

void Self::setInsetLeft(float inset) {
    insetLeft_ = inset;
    updateInset();
}

void Self::setInsetTop(float inset) {
    insetTop_ = inset;
    updateInset();
}

void Self::setInsetRight(float inset) {
    insetRight_ = inset;
    updateInset();
}

void Self::setInsetBottom(float inset) {
    insetBottom_ = inset;
    updateInset();
}

const cocos2d::Size& Self::getInnerContentSize() const {
    return container_->getContentSize();
}

void Self::onSizeChanged() {
    Super::onSizeChanged();
    updateInset();
}

void Self::updateInset() {
    auto position = cocos2d::Point(insetLeft_, insetBottom_);
    auto size = cocos2d::Size(
        std::max(0.0f, getContentSize().width - insetLeft_ - insetRight_),
        std::max(0.0f, getContentSize().height - insetTop_ - insetBottom_));
    container_->setPosition(position + size / 2);
    container_->setContentSize(size);

#ifndef NDEBUG
    drawNode_->clear();
    drawNode_->drawRect(cocos2d::Point::ZERO, getContentSize(),
                        cocos2d::Color4F::BLUE);
    drawNode_->drawRect(position, position + size, cocos2d::Color4F::RED);
#endif // NDEBUG
}
} // namespace ui
} // namespace ee
