//
//  PluginProtocol.m
//  ee-core
//
//  Created by Zinge on 7/1/16.
//
//

#include "ee/internal/MessageBridge.hpp"

#import "ee/internal/EEMessageBridge.h"

namespace ee {
std::string MessageBridge::call(const std::string& tag,
                                const std::string& msg) {
    NSString* result =
        [[EEMessageBridge getInstance] call:@(tag.c_str()) msg:@(msg.c_str())];
    return [result UTF8String];
}
} // namespace ee
