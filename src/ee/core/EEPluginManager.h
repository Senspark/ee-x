//
//  EEPluginManager.hpp
//  ee_core
//
//  Created by Zinge on 3/29/17.
//
//

#import <Foundation/Foundation.h>

#import "ee/core/EEIPlugin.h"

@interface EEPluginManager : NSObject

+ (instancetype _Nonnull)getInstance;

- (void)initializePlugins;

/// Adds and initialize a plugin.
/// @param[in] pluginName The plugin's name, e.g. AdMob, Vungle.
- (void)addPlugin:(NSString* _Nonnull)pluginName;

/// Removes and deinitialize a plugin.
/// @param[in] pluginName The plugin's name, e.g. AdMob, Vungle.
- (void)removePlugin:(NSString* _Nonnull)pluginName;

- (BOOL)application:(UIApplication* _Nonnull)application
            openURL:(NSURL* _Nonnull)url
            options:(NSDictionary* _Nonnull)options;

- (BOOL)application:(UIApplication* _Nonnull)application
              openURL:(NSURL* _Nonnull)url
    sourceApplication:(NSString* _Nonnull)sourceApplication
           annotation:(id _Nonnull)annotation;

- (BOOL)application:(UIApplication* _Nonnull)application
    continueUserActivity:(NSUserActivity* _Nonnull)userActivity
      restorationHandler:(EERestorationHandler _Nonnull)restorationHandler;

@end
