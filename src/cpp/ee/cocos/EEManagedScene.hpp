//
//  EEManagedScene.hpp
//  ee-library
//
//  Created by Zinge on 4/24/17.
//
//

#ifndef EE_LIBRARY_MANAGED_SCENE_HPP_
#define EE_LIBRARY_MANAGED_SCENE_HPP_

#ifdef __cplusplus

#include "ee/cocos/CocosFwd.hpp"

#ifdef EE_X_COCOS_CPP
#include <2d/CCScene.h>

namespace ee {
class ManagedScene : public cocos2d::Scene {
private:
    using Self = ManagedScene;
    using Super = cocos2d::Scene;

public:
    CREATE_FUNC(Self);

    virtual ~ManagedScene() override;

protected:
    ManagedScene();

    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onExit() override;

private:
    friend SceneSwitcher;

    bool unloaded_;

    void unloadItems();

    std::vector<ImageBuilder> images_;
};
} // namespace ee

#endif // EE_X_COCOS_CPP
#endif // __cplusplus

#endif /* EE_LIBRARY_MANAGED_SCENE_HPP_ */
