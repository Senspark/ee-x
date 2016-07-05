//
//  __ParseNativeBridge.hpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import <Foundation/Foundation.h>

@interface EEPluginProtocol : NSObject

@property (atomic, readonly, nonnull) NSString* pluginName;

/// Sends a JSON formatted message to C++ side.
- (void) sendCppMessage:(NSString* _Nonnull) msg
         withCallbackId:(NSNumber* _Nonnull) callbackId;

- (void) sendCppMessage:(NSString* _Nonnull) msg
                withTag:(NSString* _Nonnull) tag;

- (UIViewController* _Nullable) getCurrentRootViewController;

@end