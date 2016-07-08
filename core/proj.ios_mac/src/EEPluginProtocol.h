//
//  __ParseNativeBridge.hpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import <Foundation/Foundation.h>

#if TARGET_OS_IOS
@class UIViewController;
#endif

@interface EEPluginProtocol : NSObject

@property (atomic, readonly, nonnull) NSString* pluginName;

// clang-format off

/// Sends a JSON formatted message to C++ side.
- (void) sendCppMessage:(NSString* _Nonnull) msg
         withCallbackId:(NSNumber* _Nonnull) callbackId;

- (void) sendCppMessage:(NSString* _Nonnull) msg
                withTag:(NSString* _Nonnull) tag;

#if TARGET_OS_IOS
- (UIViewController* _Nullable) getCurrentRootViewController;
#endif

// clang-format on

@end