//
//  EEDialog.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 9/1/15.
//
//

#include "ee/cocos/Dialog.hpp"

#ifdef EE_X_COCOS_CPP
#include <2d/CCAction.h>
#include <base/CCDirector.h>

#include <ee/core/Logger.hpp>

#include "ee/cocos/EEDialogManager.hpp"
#include "ee/cocos/EEUtils.hpp"
#include "ee/cocos/IDialogManager.hpp"

namespace ee {
namespace cocos {
using Self = Dialog;

const int Self::ContainerLocalZOrder = 123456;
const std::size_t Self::TopLevel = 123456;

Self::Dialog()
    : dialogLevel_(0)
    , isActive_(false)
    , isLocked_(false)
    , ignoreTouchOutside_(false)
    , transitionAction_(nullptr)
    , logger_(Logger::getSystemLogger()) {}

Self::~Dialog() = default;

bool Self::init() {
    if (not Super::init()) {
        return false;
    }

    transitionAction_ = cocos2d::Node::create();
    transitionAction_->setVisible(false);
    addProtectedChild(transitionAction_);

    addClickEventListener(CC_CALLBACK_0(Self::onPressed, this));
    return true;
}

void Self::onEnter() {
    Super::onEnter();
}

void Self::onExit() {
    Super::onExit();
}

const cocos2d::Node* Self::getContainer() const {
    return getParent();
}

cocos2d::Node* Self::getContainer() {
    return getParent();
}

void Self::show(std::size_t level) {
    if (isActive_) {
        // No effect.
        logger_.warn("%s: attempted to show an active dialog = %p",
                     __PRETTY_FUNCTION__, this);
        CC_ASSERT(false);
        return;
    }
    if (isLocked_) {
        // No effect.
        logger_.warn("%s: attempted to show a locked dialog = %p",
                     __PRETTY_FUNCTION__, this);
        CC_ASSERT(false);
        return;
    }
    isLocked_ = true;

    if (getContainer() == nullptr) {
        auto container = cocos2d::Node::create();
        container->setContentSize(_director->getWinSize());
        container->addChild(this);
    }
    dialogLevel_ = level;

    CC_ASSERT(manager_ == nullptr);
    manager_ = LegacyDialogManager::getInstance();
    manager_->pushDialog(this, level);
}

void Self::hide() {
    if (not isActive_) {
        // No effect.
        logger_.warn("%s: attempted to hide an inactive dialog = %p",
                     __PRETTY_FUNCTION__, this);
        CC_ASSERT(false);
        return;
    }
    if (isLocked_) {
        // No effect.
        logger_.warn("%s: attempted to hide a locked dialog = %p",
                     __PRETTY_FUNCTION__, this);
        CC_ASSERT(false);
        return;
    }
    isLocked_ = true;

    CC_ASSERT(manager_ != nullptr);
    manager_->popDialog(this);
    manager_.reset();
}

Self* Self::addDialogWillShowCallback(const TransitionCallback& callback,
                                      int priority) {
    dialogWillShowCallbacks_.emplace_back(callback, priority);
    return this;
}

Self* Self::addDialogDidShowCallback(const TransitionCallback& callback,
                                     int priority) {
    dialogDidShowCallbacks_.emplace_back(callback, priority);
    return this;
}

Self* Self::addDialogWillHideCallback(const TransitionCallback& callback,
                                      int priority) {
    dialogWillHideCallbacks_.emplace_back(callback, priority);
    return this;
}

Self* Self::addDialogDidHideCallback(const TransitionCallback& callback,
                                     int priority) {
    dialogDidHideCallbacks_.emplace_back(callback, priority);
    return this;
}

std::size_t Self::getDialogLevel() const noexcept {
    return dialogLevel_;
}

bool Self::isActive() const noexcept {
    return isActive_;
}

bool Self::isLocked() const noexcept {
    return isLocked_;
}

void Self::setIgnoreTouchOutside(bool ignore) {
    ignoreTouchOutside_ = ignore;
}

bool Self::isIgnoreTouchOutside() noexcept {
    return ignoreTouchOutside_;
}

bool Self::hitTest(const cocos2d::Point& pt, const cocos2d::Camera* camera,
                   cocos2d::Vec3* p) const {
    // Swallow all touches.
    return true;
}

void Self::onDialogWillShow() {
    if (not isLocked_) {
        logger_.error("%s: expected to be locked: dialog = %p",
                      __PRETTY_FUNCTION__, this);
        CC_ASSERT(false);
    }
    logger_.debug("%s: dialog = %p", __PRETTY_FUNCTION__, this);
    invokeCallbacks(dialogWillShowCallbacks_);
}

void Self::onDialogDidShow() {
    if (not isLocked_) {
        logger_.error("%s: expected to be locked: dialog = %p",
                      __PRETTY_FUNCTION__, this);
        CC_ASSERT(false);
    }
    logger_.debug("%s: dialog = %p", __PRETTY_FUNCTION__, this);
    isActive_ = true;
    isLocked_ = false;
    invokeCallbacks(dialogDidShowCallbacks_);
}

void Self::onDialogWillHide() {
    if (not isLocked_) {
        logger_.error("%s: expected to be locked: dialog = %p",
                      __PRETTY_FUNCTION__, this);
        CC_ASSERT(false);
    }
    logger_.debug("%s: dialog = %p", __PRETTY_FUNCTION__, this);
    invokeCallbacks(dialogWillHideCallbacks_);
    isActive_ = false;
}

void Self::onDialogDidHide() {
    if (not isLocked_) {
        logger_.error("%s: expected to be locked: dialog = %p",
                      __PRETTY_FUNCTION__, this);
        CC_ASSERT(false);
    }
    logger_.debug("%s: dialog = %p", __PRETTY_FUNCTION__, this);
    isLocked_ = false;
    invokeCallbacks(dialogDidHideCallbacks_);
}

void Self::invokeCallbacks(std::vector<CallbackInfo>& callbacks) {
    std::stable_sort(callbacks.begin(), callbacks.end(), Compare2nd<>());
    RefGuard guard(this);
    for (auto&& info : callbacks) {
        info.first(this);
    }
}

void Self::onClickedOutside() {
    if (isActive() && !isIgnoreTouchOutside()) {
        hide();
    }
}

void Self::onPressed() {
    auto&& position = convertToNodeSpace(getTouchEndPosition());
    cocos2d::Rect bounds(cocos2d::Point::ZERO, getContentSize());
    if (not bounds.containsPoint(position)) {
        onClickedOutside();
    }
}

Self* Self::addShowingTransition(const TransitionType& transition) {
    showingTransitions_.emplace_back(transition);
    return this;
}

Self* Self::addHidingTransition(const TransitionType& transition) {
    hidingTransitions_.emplace_back(transition);
    return this;
}
} // namespace cocos
} // namespace ee

#endif // EE_X_COCOS_CPP