//
//  EEDialogManager.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 8/27/15.
//
//

#include "EEDialogManager.hpp"
#include "EEDialogTree.hpp"

#include <base/CCDirector.h>
#include <base/CCScheduler.h>

namespace ee {
namespace dialog {
DialogManager* DialogManager::getInstance() {
    static Self sharedInstance;
    return &sharedInstance;
}

DialogManager::DialogManager() {
    cocos2d::Director::getInstance()->getScheduler()->schedule(
        std::bind(&Self::updateTree, this), this, 0.0f, false, "___update");
}

DialogManager::~DialogManager() {
    cocos2d::Director::getInstance()->getScheduler()->unschedule("___update",
                                                                 this);
}

void DialogManager::pushDialog(Dialog* dialog) {
    getCurrentTree()->pushDialog(dialog);
}

void DialogManager::pushDialog(Dialog* dialog, std::size_t level) {
    getCurrentTree()->pushDialog(dialog, level);
}

void DialogManager::popDialog(Dialog* dialog) {
    getCurrentTree()->popDialog(dialog);
}

void DialogManager::popDialog() {
    getCurrentTree()->popDialog();
}

Dialog* DialogManager::getDialog(std::size_t level) {
    return getCurrentTree()->getDialog(level);
}

Dialog* DialogManager::getTopDialog() {
    return getCurrentTree()->getTopDialog();
}

std::size_t DialogManager::getTopDialogLevel() {
    return getCurrentTree()->getLevel();
}

void DialogManager::updateTree() {
    for (auto iter = trees_.cbegin(); iter != trees_.cend();) {
        auto&& scene = iter->first;
        if (scene->getReferenceCount() == 1) {
            // Already released.
            iter = trees_.erase(iter);
        } else {
            ++iter;
        }
    }
}

std::unique_ptr<DialogTree>& DialogManager::getTree(cocos2d::Scene* scene) {
    auto iter = trees_.find(scene);
    if (iter == trees_.cend()) {
        // Lazy instantiate.
        iter = trees_.emplace(scene, std::make_unique<DialogTree>(scene)).first;
    }
    return iter->second;
}

std::unique_ptr<DialogTree>& DialogManager::getCurrentTree() {
    auto currentScene = cocos2d::Director::getInstance()->getRunningScene();
    return getTree(currentScene);
}
} // namespace dialog
} // namespac ee
