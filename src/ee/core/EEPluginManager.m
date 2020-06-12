//
//  EEPluginManager.cpp
//  ee_core
//
//  Created by Zinge on 3/29/17.
//
//

/*
#import "ee/core/EEPluginManager.h"

#import <ee_x-Swift.h>

#import "ee/core/private/EEVideoPlayerManager.h"

@implementation EEPluginManager {
    id<EEIMessageBridge> bridge_;
    NSMutableDictionary<NSString*, id<EEIPlugin>>* plugins_;
}

- (id)init {
    self = [super init];
    if (self == nil) {
        return nil;
    }
    bridge_ = [EEMessageBridge getInstance];
    plugins_ = [[NSMutableDictionary alloc] init];
    return self;
}

- (void)dealloc {
    [plugins_ release];
    plugins_ = nil;
    [super dealloc];
}

- (void)initializePlugins {
    [EEUtils registerHandlers:bridge_];
    [EEVideoPlayerManager registerHandlers];
}

- (void)addPlugin:(NSString* _Nonnull)pluginName {
    if ([plugins_ objectForKey:pluginName] != nil) {
        NSAssert(NO, @"");
        return;
    }
    Class clazz =
        NSClassFromString([NSString stringWithFormat:@"EE%@", pluginName]);
    if ([clazz conformsToProtocol:@protocol(EEIPlugin)]) {
        // Objective-C.
        id plugin = [[[clazz alloc] init] autorelease];
        [plugins_ setObject:plugin forKey:pluginName];
    } else {
        // Swift.
        id plugin = [[[clazz alloc] init: bridge_] autorelease];
        [plugins_ setObject:plugin forKey:pluginName];
    }
}

- (void)removePlugin:(NSString* _Nonnull)pluginName {
    if ([plugins_ objectForKey:pluginName] == nil) {
        NSAssert(NO, @"");
        return;
    }
    [plugins_ removeObjectForKey:pluginName];
}

- (BOOL)application:(UIApplication* _Nonnull)application
            openURL:(NSURL* _Nonnull)url
            options:(NSDictionary* _Nonnull)options {
    BOOL result = NO;
    for (id<EEIPlugin> plugin in [plugins_ allValues]) {
        if ([plugin
                respondsToSelector:@selector(application:openURL:options:)]) {
            if ([plugin application:application openURL:url options:options]) {
                result = YES;
            }
        }
    }
    return result;
}

- (BOOL)application:(UIApplication* _Nonnull)application
              openURL:(NSURL* _Nonnull)url
    sourceApplication:(NSString* _Nonnull)sourceApplication
           annotation:(id _Nonnull)annotation {
    BOOL result = NO;
    for (id<EEIPlugin> plugin in [plugins_ allValues]) {
        if ([plugin respondsToSelector:@selector
                    (application:openURL:sourceApplication:annotation:)]) {
            if ([plugin application:application
                              openURL:url
                    sourceApplication:sourceApplication
                           annotation:annotation]) {
                result = YES;
            }
        }
    }
    return result;
}

- (BOOL)application:(UIApplication* _Nonnull)application
    continueUserActivity:(NSUserActivity* _Nonnull)userActivity
      restorationHandler:(EERestorationHandler _Nonnull)restorationHandler {
    BOOL result = NO;
    for (id<EEIPlugin> plugin in [plugins_ allValues]) {
        if ([plugin respondsToSelector:@selector
                    (application:continueUserActivity:restorationHandler:)]) {
            if ([plugin application:application
                    continueUserActivity:userActivity
                      restorationHandler:restorationHandler]) {
                result = YES;
            }
        }
    }
    return result;
}

@end
*/
