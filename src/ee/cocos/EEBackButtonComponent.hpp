//
//  EEBackButtonComponent.hpp
//  ee-library
//
//  Created by Zinge on 12/27/16.
//
//

#ifndef EE_LIBRARY_BACK_BUTTON_COMPONENT_HPP_
#define EE_LIBRARY_BACK_BUTTON_COMPONENT_HPP_

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"

#include <2d/CCComponent.h>

NS_EE_BEGIN
class BackButtonComponent : public cocos2d::Component {
private:
    using Self = BackButtonComponent;
    using Super = cocos2d::Component;

public:
    using BackButtonCallback = std::function<void()>;

    static Self* create(const BackButtonCallback& callback);

    virtual void onAdd() override;
    virtual void onRemove() override;

protected:
    BackButtonComponent();
    
    virtual bool initWithCallback(const BackButtonCallback& callback);

private:
    /// Keyboard event callback.
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                       cocos2d::Event* event);

    cocos2d::EventListenerKeyboard* listener_;

    BackButtonCallback callback_;
};
NS_EE_END

#endif /* EE_LIBRARY_BACK_BUTTON_COMPONENT_HPP_ */
