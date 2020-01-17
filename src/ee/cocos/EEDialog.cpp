//
//  EEDialog.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 9/1/15.
//
//

#include "EEDialog.hpp"
#include "EEDialogManager.hpp"
#include "EEUtils.hpp"

#include <2d/CCAction.h>
#include <base/CCDirector.h>

namespace ee {
namespace dialog {
using Self = Dialog;

const int Dialog::ContainerLocalZOrder = 123456;
const std::size_t Dialog::TopLevel = 123456;

Dialog::Dialog()
    : dialogLevel_(0)
    , isActive_(false)
    , transitionAction_(nullptr)
    , ignoreTouchOutside_(false) {}

Dialog::~Dialog() = default;

bool Dialog::init() {
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

void Dialog::onEnter() {
    Super::onEnter();
}

void Dialog::onExit() {
    Super::onExit();
}

const cocos2d::Node* Dialog::getContainer() const {
    return getParent();
}

cocos2d::Node* Dialog::getContainer() {
    return getParent();
}

void Dialog::show(std::size_t level) {
    if (getContainer() == nullptr) {
        auto container = cocos2d::Node::create();
        container->setContentSize(_director->getWinSize());
        container->addChild(this);
    }

    dialogLevel_ = level;
    DialogManager::getInstance()->pushDialog(this, level);
}

void Dialog::hide() {
    DialogManager::getInstance()->popDialog(this);
}

Dialog* Dialog::addDialogWillShowCallback(const TransitionCallback& callback,
                                          int priority) {
    dialogWillShowCallbacks_.emplace_back(callback, priority);
    return this;
}

Dialog* Dialog::addDialogDidShowCallback(const TransitionCallback& callback,
                                         int priority) {
    dialogDidShowCallbacks_.emplace_back(callback, priority);
    return this;
}

Dialog* Dialog::addDialogWillHideCallback(const TransitionCallback& callback,
                                          int priority) {
    dialogWillHideCallbacks_.emplace_back(callback, priority);
    return this;
}

Dialog* Dialog::addDialogDidHideCallback(const TransitionCallback& callback,
                                         int priority) {
    dialogDidHideCallbacks_.emplace_back(callback, priority);
    return this;
}

std::size_t Dialog::getDialogLevel() const noexcept {
    return dialogLevel_;
}

void Dialog::setActive(bool active) {
    isActive_ = active;
}

bool Dialog::isActive() const noexcept {
    return isActive_;
}

void Dialog::setIgnoreTouchOutside(bool ignore) {
    ignoreTouchOutside_ = ignore;
}

bool Dialog::isIgnoreTouchOutside() noexcept {
    return ignoreTouchOutside_;
}

bool Dialog::hitTest(const cocos2d::Point& pt, const cocos2d::Camera* camera,
                     cocos2d::Vec3* p) const {
    // Swallow all touches.
    return true;
}

void Dialog::onDialogWillShow() {
    LOG_FUNC_FORMAT("dialog = %p", this);
    invokeCallbacks(dialogWillShowCallbacks_);
}

void Dialog::onDialogDidShow() {
    LOG_FUNC_FORMAT("dialog = %p", this);
    invokeCallbacks(dialogDidShowCallbacks_);
}

void Dialog::onDialogWillHide() {
    LOG_FUNC_FORMAT("dialog = %p", this);
    invokeCallbacks(dialogWillHideCallbacks_);
}

void Dialog::onDialogDidHide() {
    LOG_FUNC_FORMAT("dialog = %p", this);
    invokeCallbacks(dialogDidHideCallbacks_);
}

void Dialog::invokeCallbacks(std::vector<CallbackInfo>& callbacks) {
    std::stable_sort(callbacks.begin(), callbacks.end(), Compare2nd<>());
    RefGuard guard(this);
    for (auto&& info : callbacks) {
        info.first(this);
    }
}

void Dialog::onClickedOutside() {
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

Dialog* Dialog::addShowingTransition(const TransitionType& transition) {
    showingTransitions_.emplace_back(transition);
    return this;
}

Dialog* Dialog::addHidingTransition(const TransitionType& transition) {
    hidingTransitions_.emplace_back(transition);
    return this;
}
} // namespace dialog
} // namespace ee
