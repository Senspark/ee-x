//
//  PluginProtocol.m
//  ee-core
//
//  Created by Zinge on 7/1/16.
//
//

#import "ee/core/EEMessageBridge.h"
#import "ee/core/MessageBridge.hpp"

@implementation EEMessageBridge (Cpp)

- (NSString* _Nonnull)callCppInternal:(NSString* _Nonnull)tag
                              message:(NSString* _Nonnull)message {
    auto&& bridge = ee::MessageBridge::getInstance();
    auto result = bridge.callCpp([tag UTF8String], [message UTF8String]);
    return (NSString * _Nonnull)[NSString stringWithUTF8String:result.c_str()];
}

@end

namespace ee {
namespace core {
std::string MessageBridge::call(const std::string& tag,
                                const std::string& message) {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    NSString* result = [bridge call:@(tag.c_str()) message:@(message.c_str())];
    return [result UTF8String];
}
} // namespace core
} // namespace ee
