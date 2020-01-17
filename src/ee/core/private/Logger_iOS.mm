//
//  Logger-iOS.m
//  ee-core
//
//  Created by Zinge on 7/6/16.
//
//

#include "ee/core/Utils.hpp"

#import <Foundation/Foundation.h>

#include "ee/core/LogLevel.hpp"

namespace ee {
namespace core {
void log(const LogLevel& level, const std::string& tag,
         const std::string& message) {
    NSLog(@"%s %s: %s", level.desc.c_str(), tag.c_str(), message.c_str());
}
} // namespace core
} // namespace ee
