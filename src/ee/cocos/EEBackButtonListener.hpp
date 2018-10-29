//
//  EEBackButtonListener.hpp
//  ee-library
//
//  Created by enrevol on 10/26/15.
//
//

#ifndef EE_LIBRARY_BACK_BUTTON_LISTENER_HPP_
#define EE_LIBRARY_BACK_BUTTON_LISTENER_HPP_

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"

#include <base/CCEventListenerKeyboard.h>

NS_EE_BEGIN
NS_DETAIL_BEGIN
class BackButtonListenerBase {
protected:
    BackButtonListenerBase() = default;
    virtual ~BackButtonListenerBase() = default;

    /// Register the keyboard listener for back button.
    void registerListener(cocos2d::Node* node);

    /// Unregister the listener.
    void unregisterListener();

    /// Keyboard event callback.
    void onKeyReleased(cocos2d::Node* node,
                       cocos2d::EventKeyboard::KeyCode keyCode,
                       cocos2d::Event* event);

    /// Callback method when the user presses back button on Android.
    ///
    /// Invoked when subclass instance is running
    /// (between @c onEnter() and @c onExit()).
    ///
    /// Override this method to handle your back button mechanism.
    ///
    /// Example:
    /// @code
    /// class YourClass : public ee::BackButtonListener<YourClass> {
    ///     ...
    ///     virtual void onBackButtonPressed() override {
    ///         // Handle your back button mechanism here.
    ///     }
    ///     ...
    /// };
    /// @endcode
    virtual void onBackButtonPressed() = 0;

private:
    cocos2d::EventListenerKeyboard* listener_;
};
NS_DETAIL_END

/// Back button listener (only works on Android).
///
/// Only one back button listener instance
/// can receive the touch at a time (swallow touch).
template <class T>
class[[deprecated]] BackButtonListener : public detail::BackButtonListenerBase {
protected:
    BackButtonListener() : BackButtonListenerBase() {
        registerListener(getInstance());
    }

    virtual ~BackButtonListener() {
        unregisterListener();
    }

    using BackButtonListenerBase::onBackButtonPressed;

private:
    /// Retrieves the subclass instance.
    ///
    /// Subclass should be inherits from @c cocos2d::Node.
    ///
    /// Since dynamic_cast won't work in constructor,
    /// static_cast is used.
    T* getInstance() {
        return static_cast<T*>(this);
    }

    /// Retrieves the subclass instance (const).
    ///
    /// Subclass should be inherits from @c cocos2d::Node.
    const T* getInstance() const {
        return static_cast<const T*>(this);
    }
};
NS_EE_END

#endif /* EE_LIBRARY_BACK_BUTTON_LISTENER_HPP_ */
