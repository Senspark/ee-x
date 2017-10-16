//
//  AppController.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#import <ee/Core.hpp>

#import "AppController.h"
#import "AppDelegate.hpp"
#import "RootViewController.h"

#import <cocos2d.h>

@implementation AppController

@synthesize window;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static eetest::AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {
    auto app = cocos2d::Application::getInstance();

    // Initialize the GLView attributes.
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();

    // Override point for customization after application launch.

    // Use RootViewController to manage CCEAGLView
    _viewController = [[[RootViewController alloc] init] autorelease];
    [[self viewController] setExtendedLayoutIncludesOpaqueBars:YES];
    [[self viewController] setEdgesForExtendedLayout:UIRectEdgeAll];

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    [window setRootViewController:[self viewController]];
    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];

    // IMPORTANT: Setting the GLView should be done after creating the
    // RootViewController
    auto glview = cocos2d::GLViewImpl::createWithEAGLView(
        (__bridge void*)[_viewController view]);
    cocos2d::Director::getInstance()->setOpenGLView(glview);

    [EEMetrics initializeMetrics];
    EEPluginManager* manager = [EEPluginManager getInstance];
    [manager addPlugin:@"Crashlytics"];
    [manager addPlugin:@"Notification"];
    [manager addPlugin:@"AdMob"];
    [manager addPlugin:@"AppLovin"];
    [manager addPlugin:@"FacebookAds"];
    [manager addPlugin:@"IronSource"];
    [manager addPlugin:@"UnityAds"];
    [manager addPlugin:@"Vungle"];

    // run the cocos2d-x game scene
    app->run();

    return YES;
}

- (void)applicationWillResignActive:(UIApplication*)application {
    /*
     Sent when the application is about to move from active to inactive state.
     This can occur for certain types of temporary interruptions (such as an
     incoming phone call or SMS message) or when the user quits the application
     and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down
     OpenGL ES frame rates. Games should use this method to pause the game.
     */
    // We don't need to call this method any more. It will interrupt user
    // defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication*)application {
    /*
     Restart any tasks that were paused (or not yet started) while the
     application was inactive. If the application was previously in the
     background, optionally refresh the user interface.
     */
    // We don't need to call this method any more. It will interrupt user
    // defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
}

- (void)applicationDidEnterBackground:(UIApplication*)application {
    /*
     Use this method to release shared resources, save user data, invalidate
     timers, and store enough application state information to restore your
     application to its current state in case it is terminated later.
     If your application supports background execution, called instead of
     applicationWillTerminate: when the user quits.
     */
    if ([[self viewController] isActive]) {
        cocos2d::Application::getInstance()->applicationDidEnterBackground();
    }
}

- (void)applicationWillEnterForeground:(UIApplication*)application {
    /*
     Called as part of  transition from the background to the inactive state:
     here you can undo many of the changes made on entering the background.
     */
    if ([[self viewController] isActive]) {
        cocos2d::Application::getInstance()->applicationWillEnterForeground();
    }
}

- (void)applicationWillTerminate:(UIApplication*)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication*)application {
    /*
     Free up as much memory as possible by purging cached data objects that can
     be recreated (or reloaded from disk) later.
     */
}

- (void)dealloc {
    [window release];
    [_viewController release];
    [super dealloc];
}

@end
