//
//  PluginProtocol.m
//  ee-core
//
//  Created by Zinge on 7/1/16.
//
//

#import "ee/core/internal/MessageBridge.hpp"

#import "ee/core/internal/MessageBridge_Swift.h"

extern "C" {
const char* ee_callCppInternal(const char* tag, const char* message) {
    auto&& bridge = ee::MessageBridge::getInstance();
    auto result = bridge.callCpp(tag, message);
    auto result_chars = new char[result.size() + 1];
    std::copy(result.cbegin(), result.cend(), result_chars);
    result_chars[result.size()] = '\0';
    return result_chars;
}

void ee_freeString(const char* str) {
    delete[] str;
}

char* ee_staticCall(const char* tag, const char* message);
} // extern "C"

namespace ee {
namespace core {
std::string MessageBridge::call(const std::string& tag,
                                const std::string& message) {
    auto response_swift = ee_staticCall(tag.c_str(), message.c_str());
    if (response_swift == nullptr) {
        return "";
    }
    auto result = std::string(response_swift);
    std::free(response_swift);
    return result;
}
} // namespace core
} // namespace ee
