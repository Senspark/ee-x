//
//  Utils_iOS.m
//  ee_x
//
//  Created by Zinge on 10/18/17.
//
//

#import <Foundation/Foundation.h>

#import "ee/core/Utils.hpp"

namespace ee {
namespace core {
void runOnUiThread(const Runnable<void>& runnable) {
    if ([NSThread isMainThread]) {
        runnable();
    } else {
        // Capture lambda into Objective-C block.
        struct {
            Runnable<void> runnable;
        } lambda{runnable};
        dispatch_async(dispatch_get_main_queue(), ^{
            lambda.runnable();
        });
    }
}
} // namespace core
} // namespace ee
