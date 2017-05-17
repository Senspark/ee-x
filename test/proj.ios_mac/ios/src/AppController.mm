//
//  AppController.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#import "AppController.h"

#import <ee/Core.hpp>

#include "NotificationAgent.hpp"

@implementation AppController

- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {

    [[EEPluginManager getInstance] addPlugin:@"Crashlytics"];
    [[EEPluginManager getInstance] addPlugin:@"Notification"];

    return YES;
}

- (void)applicationDidEnterBackground:(UIApplication*)application {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    // Invoke
    // cocos2d::Application::getInstance()->applicationDidEnterBackground();

    NotificationAgent::getInstance()->scheduleAll();
}

- (void)applicationWillEnterForeground:(UIApplication*)application {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    // Invoke
    // cocos2d::Application::getInstance()->applicationWillEnterForeground();

    NotificationAgent::getInstance()->unscheduleAll();
}

@end
