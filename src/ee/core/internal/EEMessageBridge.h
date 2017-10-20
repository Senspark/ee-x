//
//  EEPluginManager.hpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import <Foundation/Foundation.h>

typedef NSString* _Nonnull (^EEMessageHandler)(NSString* _Nonnull message);

@interface EEMessageBridge : NSObject

/// Gets a message bridge instance.
+ (instancetype _Nonnull)getInstance;

/// Registers a new handler to receive messages from C++.
/// @param handler The handler.
/// @param tag The unique tag of the handler.
/// @return Whether the registration was successful.
- (BOOL)registerHandler:(EEMessageHandler _Nonnull)handler
                    tag:(NSString* _Nonnull)tag;

/// Convenience method to fix clang-format.
- (BOOL)registerHandler:(NSString* _Nonnull)tag
               callback:(EEMessageHandler _Nonnull)handler;

/// Deregisters an existing handler not to receive messages from C++.
/// @param tag The unique tag of the handler.
/// @return Whether the deregistration was successful.
- (BOOL)deregisterHandler:(NSString* _Nonnull)tag;

/// Calls a handler from Objective-C with a message.
/// @warning This method should not be called manually.
/// @param tag The tag of the handler.
/// @param message The message.
/// @return Reply message from Objective-C.
- (NSString* _Nonnull)call:(NSString* _Nonnull)tag
                   message:(NSString* _Nonnull)message;

@end

@interface EEMessageBridge (Cpp)

/// Calls a handler from C++ without a message.
/// @param tag The unique tag of the handler.
/// @return Reply message from C++.
- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag;

/// Calls a handler from C++ with a message.
/// @param tag The unique tag of the handler.
/// @param message The message.
/// @return Reply message from C++.
- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag
                      message:(NSString* _Nonnull)message;

@end
