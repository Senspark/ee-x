//
//  EEPluginManager.hpp
//  ee_core
//
//  Created by Zinge on 3/29/17.
//
//

#import <Foundation/Foundation.h>

@interface EEPluginManager : NSObject

+ (instancetype _Nonnull)getInstance;

/// Adds and initialize a plugin.
/// @param[in] pluginName The plugin's name, e.g. AdMob, Vungle.
- (void)addPlugin:(NSString* _Nonnull)pluginName;

/// Removes and deinitialize a plugin.
/// @param[in] pluginName The plugin's name, e.g. AdMob, Vungle.
- (void)removePlugin:(NSString* _Nonnull)pluginName;

@end
