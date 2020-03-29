//
//  EEDialogManager.cpp
//  roll-eat
//
//  Created by Hoang Le Hai on 8/27/15.
//
//

#include "ee/cocos/EEDialogManager.hpp"

#include <2d/CCTransition.h>
#include <base/CCDirector.h>

#include "ee/cocos/DialogManager.hpp"

namespace ee {
namespace cocos {
class TreeComponent final : public cocos2d::Component {
private:
    using Self = TreeComponent;
    using Super = cocos2d::Component;

public:
    static const std::string Name;

    CREATE_FUNC(Self);

    const std::shared_ptr<IDialogManager>& getManager() const { //
        return manager_;
    }

private:
    TreeComponent() { //
        setName(Name);
    }

    virtual void onAdd() override {
        Super::onAdd();
        manager_ = std::make_shared<DialogManager>(getOwner());
    }

    virtual void onRemove() override {
        Super::onRemove();
        manager_.reset();
    }

    std::shared_ptr<IDialogManager> manager_;
};

const std::string TreeComponent::Name = "_ee_x_tree_component_";

namespace {
auto getCurrentScene() {
    auto currentScene = cocos2d::Director::getInstance()->getRunningScene();
    return currentScene;
}
} // namespace

using Self = LegacyDialogManager;

const std::shared_ptr<IDialogManager>& Self::getInstance() {
    auto currentScene = getCurrentScene();
    CC_ASSERT(dynamic_cast<cocos2d::TransitionScene*>(currentScene) == nullptr);
    return getManager(currentScene);
}

const std::shared_ptr<IDialogManager>& Self::getManager(cocos2d::Scene* scene) {
    auto component = scene->getComponent(TreeComponent::Name);
    if (component == nullptr) {
        component = TreeComponent::create();
        scene->addComponent(component);
    }
    auto treeComponent = dynamic_cast<TreeComponent*>(component);
    return treeComponent->getManager();
}
} // namespace cocos
} // namespace ee
