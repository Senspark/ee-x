//
//  PluginProtocol.m
//  ee-core
//
//  Created by Zinge on 7/1/16.
//
//

#include "ee/PluginProtocol.hpp"

#import "ee/EEPluginManager.h"

namespace ee {
namespace core {
std::string
PluginProtocol::callNative1(const std::string& methodName,
                            const optional<std::string>& json_,
                            const optional<int>& callbackId_) const {
    NSString* json = (json_ ? @(json_.value().c_str()) : nil);
    NSNumber* callbackId = (callbackId_ ? @(callbackId_.value()) : nil);
    NSString* result = [EEPluginManager callNative:@(pluginName_.c_str())
                                            method:@(methodName.c_str())
                                           message:json
                                        callbackId:callbackId];
    return [result UTF8String];
}
} // namespace core
} // namespace ee