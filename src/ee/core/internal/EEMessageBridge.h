//
//  EEPluginManager.hpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import <Foundation/Foundation.h>

typedef NSString* _Nonnull (^EEMessageHandler)(NSString* _Nonnull msg);

@interface EEMessageBridge : NSObject {
    /// Registered handlers.
    NSMutableDictionary* handlers;
}

/// Gets a message bridge instance.
+ (instancetype _Nonnull)getInstance;

/// Registers a new handler to receive messages from C++.
/// @param handler The handler.
/// @param tag The unique tag of the handler.
/// @return Whether the registration was successful.
- (BOOL)registerHandler:(EEMessageHandler _Nonnull)handler
                    tag:(NSString* _Nonnull)tag;

/// Deregisters an existing handler not to receive messages from C++.
/// @param tag The unique tag of the handler.
/// @return Whether the deregistration was successful.
- (BOOL)deregisterHandler:(NSString* _Nonnull)tag;

@end

@interface EEMessageBridge (Cpp)

/// Calls a handler from C++ without a message.
/// @param tag The unique tag of the handler.
/// @return Reply message from C++.
- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag;

/// Calls a handler from C++ with a message.
/// @param tag The unique tag of the handler.
/// @param msg The message.
/// @return Reply message from C++.
- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag
                          msg:(NSString* _Nonnull)msg;

@end
