//
//  AppController.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#import "AppController.h"

#import <ee/Core.hpp>

#include "AppDelegate.hpp"

@implementation AppController

- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {

    [[EEPluginManager getInstance] addPlugin:@"Crashlytics"];
    [[EEPluginManager getInstance] addPlugin:@"Notification"];

    // Simulate iOS.
    AppDelegate::getInstance()->applicationDidFinishLaunching();

    return YES;
}

- (void)applicationDidEnterBackground:(UIApplication*)application {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    // Simulate
    // cocos2d::Application::getInstance()->applicationDidEnterBackground();
    AppDelegate::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication*)application {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    // Simulate
    // cocos2d::Application::getInstance()->applicationWillEnterForeground();
    AppDelegate::getInstance()->applicationWillEnterForeground();
}

@end
