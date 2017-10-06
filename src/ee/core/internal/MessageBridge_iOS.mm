//
//  PluginProtocol.m
//  ee-core
//
//  Created by Zinge on 7/1/16.
//
//

#include "ee/core/internal/MessageBridge.hpp"

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEMessageBridge.h"

@implementation EEMessageBridge (Cpp)

- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag {
    return [self callCpp:tag message:@""];
}

- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag
                      message:(NSString* _Nonnull)message {
    auto result = ee::core::MessageBridge::getInstance().callCpp(
        [tag UTF8String], [message UTF8String]);
    return (NSString * _Nonnull)[NSString stringWithUTF8String:result.c_str()];
}

@end

@interface EEMessageBridge (PrivateCpp)

/// Calls a handler from Objective-C with a message.
/// @warning This method should not be called manually.
/// @param tag The tag of the handler.
/// @param message The message.
/// @return Reply message from Objective-C.
- (NSString* _Nonnull)call:(NSString* _Nonnull)tag
                   message:(NSString* _Nonnull)message;

@end

@implementation EEMessageBridge (PrivateCpp)

- (NSString* _Nonnull)call:(NSString* _Nonnull)tag
                   message:(NSString* _Nonnull)message {
    EEMessageHandler handler = [handlers objectForKey:tag];
    if (handler == nil) {
        NSAssert(NO, @"...");
        return @"";
    }
    return handler(message);
}

@end

namespace ee {
namespace core {
std::string MessageBridge::call(const std::string& tag,
                                const std::string& message) {
    NSString* result = [[EEMessageBridge getInstance] call:@(tag.c_str())
                                                   message:@(message.c_str())];
    return [result UTF8String];
}
} // namespace core
} // namespace ee
