//
//  PluginProtocol.m
//  ee-core
//
//  Created by Zinge on 7/1/16.
//
//

#include "ee/core/internal/MessageBridge.hpp"

#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"

@implementation EEMessageBridge (Cpp)

- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag {
    return [self callCpp:tag msg:@""];
}

- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag
                          msg:(NSString* _Nonnull)msg {
    auto result = ee::core::MessageBridge::getInstance().callCpp(
        [tag UTF8String], [msg UTF8String]);
    return (NSString * _Nonnull)[NSString stringWithUTF8String:result.c_str()];
}

@end

@interface EEMessageBridge (PrivateCpp)

/// Calls a handler from Objective-C with a message.
/// @warning This method should not be called manually.
/// @param tag The tag of the handler.
/// @param msg The message.
/// @return Reply message from Objective-C.
- (NSString* _Nonnull)call:(NSString* _Nonnull)tag msg:(NSString* _Nonnull)msg;

@end

@implementation EEMessageBridge (PrivateCpp)

- (NSString* _Nonnull)call:(NSString* _Nonnull)tag msg:(NSString* _Nonnull)msg {
    EEMessageHandler handler = [handlers objectForKey:tag];
    if (handler == nil) {
        NSAssert(NO, @"...");
        return [EEDictionaryUtils emptyResult];
    }
    return handler(msg);
}

@end

namespace ee {
namespace core {
std::string MessageBridge::call(const std::string& tag,
                                const std::string& msg) {
    NSString* result =
        [[EEMessageBridge getInstance] call:@(tag.c_str()) msg:@(msg.c_str())];
    return [result UTF8String];
}
} // namespace core
} // namespace ee
