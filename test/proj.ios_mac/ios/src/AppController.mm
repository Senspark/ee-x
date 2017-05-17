//
//  AppController.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#import "AppController.h"

#import <ee/Core.hpp>

@implementation AppController

- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {

    [[EEPluginManager getInstance] addPlugin:@"Crashlytics"];

    return YES;
}

@end
