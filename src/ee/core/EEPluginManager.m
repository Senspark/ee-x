//
//  EEPluginManager.cpp
//  ee_core
//
//  Created by Zinge on 3/29/17.
//
//

#import "ee/core/EEPluginManager.h"

#import "ee/core/internal/EEMetrics.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/core/internal/EEVideoPlayerManager.h"

@interface EEPluginManager () {
    NSMutableDictionary<NSString*, id<EEIPlugin>>* plugins_;
}

@end

@implementation EEPluginManager

+ (instancetype _Nonnull)getInstance {
    static EEPluginManager* sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

- (id)init {
    self = [super init];
    if (self == nil) {
        return nil;
    }
    plugins_ = [[NSMutableDictionary alloc] init];
    return self;
}

- (void)dealloc {
    [plugins_ release];
    plugins_ = nil;
    [super dealloc];
}

- (void)initializePlugins {
    [EEUtils registerHandlers];
    [EEMetrics registerHandlers];
    [EEVideoPlayerManager registerHandlers];
}

- (void)addPlugin:(NSString* _Nonnull)pluginName {
    if ([plugins_ objectForKey:pluginName] != nil) {
        NSAssert(NO, @"...");
        return;
    }
    Class clazz =
        NSClassFromString([NSString stringWithFormat:@"EE%@", pluginName]);
    if ([clazz conformsToProtocol:@protocol(EEIPlugin)]) {
        id plugin = [[[clazz alloc] init] autorelease];
        [plugins_ setObject:plugin forKey:pluginName];
    }
}

- (void)removePlugin:(NSString* _Nonnull)pluginName {
    if ([plugins_ objectForKey:pluginName] == nil) {
        NSAssert(NO, @"...");
        return;
    }
    [plugins_ removeObjectForKey:pluginName];
}

- (BOOL)application:(UIApplication* _Nonnull)application
            openURL:(NSURL* _Nonnull)url
            options:(NSDictionary* _Nonnull)options {
    BOOL result = NO;
    for (id<EEIPlugin> plugin in plugins_) {
        if ([plugin
                respondsToSelector:@selector(application:openURL:options:)]) {
            result =
                result ||
                [plugin application:application openURL:url options:options];
        }
    }
    return result;
}

- (BOOL)application:(UIApplication* _Nonnull)application
              openURL:(NSURL* _Nonnull)url
    sourceApplication:(NSString* _Nonnull)sourceApplication
           annotation:(id _Nonnull)annotation {
    BOOL result = NO;
    for (id<EEIPlugin> plugin in plugins_) {
        if ([plugin respondsToSelector:@selector
                    (application:openURL:sourceApplication:annotation:)]) {
            result = result || [plugin application:application
                                             openURL:url
                                   sourceApplication:sourceApplication
                                          annotation:annotation];
        }
    }
    return result;
}

- (BOOL)application:(UIApplication* _Nonnull)application
    continueUserActivity:(NSUserActivity* _Nonnull)userActivity
      restorationHandler:(EERestorationHandler _Nonnull)restorationHandler {
    BOOL result = NO;
    for (id<EEIPlugin> plugin in plugins_) {
        if ([plugin respondsToSelector:@selector
                    (application:continueUserActivity:restorationHandler:)]) {
            result = result || [plugin application:application
                                   continueUserActivity:userActivity
                                     restorationHandler:restorationHandler];
        }
    }
    return result;
}

@end
