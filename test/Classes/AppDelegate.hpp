//
//  AppDelegate.hpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#ifndef EE_X_TEST_APP_DELEGATE_HPP_
#define EE_X_TEST_APP_DELEGATE_HPP_

#include <platform/CCApplication.h>

/// The cocos2d Application.
/// Private inheritance here hides part of interface from Director.
class AppDelegate : private cocos2d::Application {
public:
    AppDelegate();
    virtual ~AppDelegate() override;

    virtual void initGLContextAttrs() override;

    /// Implement Director and Scene init code here.
    /// @return true    Initialize success, app continue.
    /// @return false   Initialize failed, app terminate.
    virtual bool applicationDidFinishLaunching() override;

    /// Called when the application moves to the background.
    virtual void applicationDidEnterBackground() override;

    /// Called when the application reenters the foreground.
    virtual void applicationWillEnterForeground() override;
};

#endif /* EE_X_TEST_APP_DELEGATE_HPP_ */
