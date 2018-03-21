//
//  EEIMessageBridge.hpp
//  ee_x
//
//  Created by eps on 3/14/18.
//

#import "ee/core/EEIMessageBridge.h"

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
- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag;

/// @see Super.
- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag
                      message:(NSString* _Nonnull)message;

@end

@interface EEMessageBridge (Cpp)

/// Calls a handler from Objective-C with a message.
/// @warning This method should not be called manually.
/// @param tag The tag of the handler.
/// @param message The message.
/// @return Reply message from Objective-C.
- (NSString* _Nonnull)call:(NSString* _Nonnull)tag
                   message:(NSString* _Nonnull)message;

/// Used internally.
- (NSString* _Nonnull)callCppInternal:(NSString* _Nonnull)tag
                              message:(NSString* _Nonnull)message;

@end
