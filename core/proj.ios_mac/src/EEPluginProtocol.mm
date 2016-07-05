//
//  __ParseNativeBridge.cpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import <UIKit/UIKit.h>

#import "EEPluginProtocol.h"

#include "PluginManager.hpp"

@implementation EEPluginProtocol

- (void) sendCppMessage:(NSString* _Nonnull) msg
         withCallbackId:(NSNumber* _Nonnull) callbackId {
    NSAssert([msg length] > 0, @"...");

    ee::core::PluginManager::onNativeCallback([[self pluginName] UTF8String],
                                              [msg UTF8String],
                                              [callbackId intValue]);
}

- (void) sendCppMessage:(NSString* _Nonnull) msg
                withTag:(NSString* _Nonnull) tag {
    NSAssert([msg length] > 0, @"...");
    NSAssert([tag length] > 0, @"...");

    ee::core::PluginManager::onNativeCallback(
        [[self pluginName] UTF8String], [msg UTF8String], [tag UTF8String]);
}

- (UIViewController* _Nullable) getCurrentRootViewController {
    // [[[[UIApplication sharedApplication] delegate] window] rootViewController];
    return [[[UIApplication sharedApplication] keyWindow] rootViewController];
}

@end