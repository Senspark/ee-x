//
//  EEDialogTree.cpp
//  ee-library
//
//  Created by Zinge on 7/27/17.
//
//

#include "ee/cocos/DialogManager.hpp"

#include <2d/CCScene.h>

#include "ee/cocos/Dialog.hpp"
#include "ee/cocos/DialogCommand.hpp"
#include "ee/cocos/EEAction.hpp"
#include "ee/cocos/EEDialogComponent.hpp"
#include "ee/cocos/EEScopeGuard.hpp"
#include "ee/cocos/EEUtils.hpp"

namespace ee {
namespace cocos {
namespace {
void pauseAllDialog(cocos2d::Node* node, Dialog* dialog) {
    doRecursively(node, [dialog](cocos2d::Node* currentNode) {
        auto component =
            currentNode->getComponent(DialogComponent::DefaultName);
        if (component == nullptr) {
            currentNode->pause();
        } else {
            auto dialogComponent = dynamic_cast<DialogComponent*>(component);
            dialogComponent->pause(dialog);
        }
    });
}

void resumeAllDialog(cocos2d::Node* node, Dialog* dialog) {
    doRecursively(node, [dialog](cocos2d::Node* currentNode) {
        auto component =
            currentNode->getComponent(DialogComponent::DefaultName);
        if (component == nullptr) {
            currentNode->resume();
        } else {
            auto dialogComponent = dynamic_cast<DialogComponent*>(component);
            dialogComponent->resume(dialog);
        }
    });
}
} // namespace

using Self = DialogManager;

Self::DialogManager(cocos2d::Node* root)
    : root_(root) {
    currentLevel_ = 0;
    lockingDialog_ = nullptr;
}

Self::~DialogManager() = default;

void Self::pushDialog(Dialog* dialog) {
    pushDialog(dialog, Dialog::TopLevel);
}

void Self::pushDialog(Dialog* dialog, std::size_t level) {
    CC_ASSERT(dialog != nullptr);
    CC_ASSERT(dialog->getContainer() != nullptr);
    LOG_FUNC_FORMAT("scene = %p dialog = %p level = %zu", scene_, dialog,
                    level);

    pushCommand(DialogCommand(DialogCommandType::Push, dialog, level));
    processCommandQueue();
}

void Self::popDialog(Dialog* dialog) {
    CC_ASSERT(dialog != nullptr);
    CC_ASSERT(dialog->getContainer() != nullptr);

    auto level = dialog->getDialogLevel();
    LOG_FUNC_FORMAT("sceen = %p dialog = %p level = %zu", scene_, dialog,
                    level);

    pushCommand(DialogCommand(DialogCommandType::Pop, dialog, level));
    processCommandQueue();
}

void Self::popDialog() {
    popDialog(getTopDialog());
}

Dialog* Self::getDialog(std::size_t level) const {
    if (dialogStack_.empty()) {
        return nullptr;
    }
    if (level == Dialog::TopLevel) {
        level = dialogStack_.size();
    }
    CC_ASSERT(1 <= level && level <= dialogStack_.size());
    return dialogStack_.at(level - 1).dialog;
}

Dialog* Self::getTopDialog() const {
    return getDialog(Dialog::TopLevel);
}

std::size_t Self::getLevel() const {
    return dialogStack_.size();
}

void Self::reset() {
    currentLevel_ = 0;
    root_ = nullptr;
    lockingDialog_ = nullptr;
    commandQueue_.clear();
    dialogStack_.clear();
}

cocos2d::Node* Self::getRunningNode() const {
    auto dialog = getTopDialog();
    if (dialog != nullptr) {
        return dialog->getParent();
    }
    return root_;
}

bool Self::isLocked() const {
    return lockingDialog_ != nullptr;
}

void Self::lock(const Dialog* dialog) {
    LOG_FUNC_FORMAT("scene = %p dialog = %p level = %zu", scene_, dialog,
                    dialog->getDialogLevel());
    CC_ASSERT(lockingDialog_ == nullptr);
    lockingDialog_ = dialog;
}

void Self::unlock(const Dialog* dialog) {
    LOG_FUNC_FORMAT("scene = %p dialog = %p level = %zu", scene_, dialog,
                    dialog->getDialogLevel());
    CC_ASSERT(lockingDialog_ == dialog);
    lockingDialog_ = nullptr;
}

bool Self::processCommandQueue() {
    if (isLocked()) {
        return false;
    }
    using DiffType = decltype(commandQueue_)::difference_type;
    for (std::size_t i = 0; i < commandQueue_.size(); ++i) {
        if (processCommand(commandQueue_.at(i))) {
            commandQueue_.erase(
                std::next(commandQueue_.begin(), static_cast<DiffType>(i)));
            return true;
        }
    }
    return false;
}

bool Self::processCommand(const DialogCommand& command) {
    if (command.type == DialogCommandType::Push) {
        return processPushCommand(command.dialog, command.level);
    }
    if (command.type == DialogCommandType::Pop) {
        return processPopCommand(command.dialog, command.level);
    }
    CC_ASSERT(false);
    return false;
}

bool Self::processPushCommand(Dialog* dialog, std::size_t level) {
    if (level == Dialog::TopLevel || level == currentLevel_ + 1) {
        pushDialogImmediately(dialog, level);
        return true;
    }
    return false;
}

bool Self::processPopCommand(Dialog* dialog, std::size_t level) {
    if (level == Dialog::TopLevel || level == currentLevel_) {
        if (dialog != getTopDialog()) {
            // Attempted to pop not the top dialog!!!
            //            CC_ASSERT(false);
            return false;
        }
        popDialogImmediately(dialog);
        return true;
    }
    return false;
}

bool Self::pushCommand(const DialogCommand& command) {
    for (auto&& cmd : commandQueue_) {
        if (cmd.type == command.type && cmd.dialog == command.dialog) {
            // Command alreay exists!!!.
            //            CC_ASSERT(false);
            return false;
        }
    }
    commandQueue_.push_back(command);
    return true;
}

void Self::pushDialogImmediately(Dialog* dialog, std::size_t level) {
    LOG_FUNC_FORMAT("scene = %p dialog = %p level = %zu", scene_, dialog,
                    level);

    lock(dialog);
    dialog->onDialogWillShow();

    auto parent = getRunningNode();
    pauseAllDialog(parent, dialog);

    ++currentLevel_;
    dialog->dialogLevel_ = currentLevel_;
    dialogStack_.emplace_back(dialog);
    parent->addChild(dialog->getContainer(), Dialog::ContainerLocalZOrder);

    auto sequence = Sequence::create();
    for (auto&& action : dialog->showingTransitions_) {
        sequence->then(cocos2d::TargetedAction::create(dialog, action));
    }

    auto unlocker = std::make_shared<ScopeGuard>([this, dialog] {
        LOG_FUNC_FORMAT("unlocker: scene = %p dialog = %p level = %zu", scene_,
                        dialog, dialog->getDialogLevel());
        unlock(dialog);
        processCommandQueue();
    });

    sequence->then([dialog, unlocker] {
        // End showing.
        dialog->setActive(true);

        // Invoke callbacks.
        dialog->onDialogDidShow();

        // Always unlock last.
        unlocker->invoke();
    });

    dialog->transitionAction_->stopAllActions();
    dialog->transitionAction_->runAction(sequence);
}

void Self::popDialogImmediately(Dialog* dialog) {
    CC_ASSERT(dialog == getTopDialog());
    LOG_FUNC_FORMAT("scene = %p dialog = %p level = %zu", scene_, dialog,
                    dialog->getDialogLevel());

    // Always lock first.
    lock(dialog);

    // Invoke callbacks.
    dialog->onDialogWillHide();

    // Begin hiding.
    dialog->setActive(false);

    auto sequence = Sequence::create();
    for (auto&& action : dialog->hidingTransitions_) {
        sequence->then(cocos2d::TargetedAction::create(dialog, action));
    }

    auto unlocker = std::make_shared<ScopeGuard>([this, dialog] {
        LOG_FUNC_FORMAT("unlocker: scene = %p dialog = %p level = %zu", scene_,
                        dialog, dialog->getDialogLevel());
        unlock(dialog);
        processCommandQueue();
    });

    sequence->then([this, dialog, unlocker] {
        auto container = dialog->getContainer();
        auto realParent = container->getParent();
        container->removeFromParent();

        // When the dialog doesn't have any hiding transition, i.e.
        // transitionAction_ won't run any TargetedAction, so the dialog
        // will not be guarded by the TargetedAction (TargetedAction will
        // keep a reference to the targeted node).
        // We have to manually guard the dialog here.
        auto guard = ee::make_ref_guard(dialog);

        dialogStack_.pop_back();
        --currentLevel_;

        auto parent = getRunningNode();
        CC_ASSERT(parent == realParent);
        resumeAllDialog(parent, dialog);

        dialog->onDialogDidHide();
        unlocker->invoke();
    });

    dialog->transitionAction_->stopAllActions();
    dialog->transitionAction_->runAction(sequence);
}
} // namespace cocos
} // namespace ee
