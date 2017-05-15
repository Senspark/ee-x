//
//  PluginProtocol.m
//  ee-core
//
//  Created by Zinge on 7/1/16.
//
//

#include "ee/core/internal/MessageBridge.hpp"

#import "ee/core/internal/EEMessageBridge.h"

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

@implementation EEMessageBridge (Cpp)

- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag
                          msg:(NSString* _Nonnull)msg {
    auto result = ee::core::MessageBridge::getInstance().call([tag UTF8String],
                                                              [msg UTF8String]);
    return (NSString * _Nonnull)[NSString stringWithUTF8String:result.c_str()];
}

@end
