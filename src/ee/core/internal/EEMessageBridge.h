//
//  EEIMessageBridge.hpp
//  ee_x
//
//  Created by eps on 3/14/18.
//

#import "ee/core/internal/EEIMessageBridge.h"

@interface EEMessageBridge : NSObject <EEIMessageBridge>

/// Gets the message bridge instance.
+ (instancetype _Nonnull)getInstance;

- (BOOL)isHandlerRegistered:(NSString* _Nonnull)tag;

/// @see Super.
- (BOOL)registerHandler:(EEMessageHandler _Nonnull)handler
                    tag:(NSString* _Nonnull)tag;

/// @see Super.
- (BOOL)registerHandler:(NSString* _Nonnull)tag
               callback:(EEMessageHandler _Nonnull)handler;

/// @see Super.
- (BOOL)deregisterHandler:(NSString* _Nonnull)tag;

/// @see Super.
- (NSString* _Nonnull)call:(NSString* _Nonnull)tag
                   message:(NSString* _Nonnull)message;

/// @see Super.
- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag;

/// @see Super.
- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag
                      message:(NSString* _Nonnull)message;

@end

@interface EEMessageBridge (Cpp)

/// Used internally.
- (NSString* _Nonnull)callCppInternal:(NSString* _Nonnull)tag
                              message:(NSString* _Nonnull)message;

@end
