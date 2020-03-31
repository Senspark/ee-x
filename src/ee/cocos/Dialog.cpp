//
//  EEDialog.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 9/1/15.
//
//

#include "ee/cocos/Dialog.hpp"

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

    setActive(false);
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

void Self::setActive(bool active) {
    isActive_ = active;
}

bool Self::isActive() const noexcept {
    return isActive_;
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
    logger_.debug("dialog = %p", this);
    invokeCallbacks(dialogWillShowCallbacks_);
}

void Self::onDialogDidShow() {
    logger_.debug("dialog = %p", this);
    invokeCallbacks(dialogDidShowCallbacks_);
}

void Self::onDialogWillHide() {
    logger_.debug("dialog = %p", this);
    invokeCallbacks(dialogWillHideCallbacks_);
}

void Self::onDialogDidHide() {
    logger_.debug("dialog = %p", this);
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
