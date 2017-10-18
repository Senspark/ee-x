//
//  Utils_iOS.m
//  ee_x
//
//  Created by Zinge on 10/18/17.
//
//

#import <Foundation/NSThread.h>
#import <dispatch/dispatch.h>

#import "ee/core/Utils.hpp"

namespace ee {
namespace core {
void runOnUiThread(const Runnable<void>& runnable) {
    if ([NSThread isMainThread]) {
        runnable();
    } else {
        dispatch_async(dispatch_get_main_queue(), ^{
            runnable();
        });
    }
}
} // namespace core
} // namespace ee
