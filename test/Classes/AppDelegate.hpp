//
//  AppDelegate.hpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#ifndef EE_X_TEST_APP_DELEGATE_HPP_
#define EE_X_TEST_APP_DELEGATE_HPP_

/// Simulates AppDelegate.
/// Used for Travis.
class AppDelegate {
public:
    static AppDelegate* getInstance();

    /// Simulates applicationDidFinishLaunching.
    bool applicationDidFinishLaunching();

    /// Simluates applicationDidEnterBackground.
    void applicationDidEnterBackground();

    /// Simulates applicationWillEnterForeground.
    void applicationWillEnterForeground();
};

#endif /* EE_X_TEST_APP_DELEGATE_HPP_ */
