//
//  EECrashlyticsProtocol.m
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#import <UIKit/UIKit.h>
#import <Crashlytics/Crashlytics.h>

#import "EECrashlyticsProtocol.h"

#undef CLS_LOG
#ifdef __OBJC__
#ifndef NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSNSLog(__FORMAT__, ##__VA_ARGS__)
#else
#define CLS_LOG(__FORMAT__, ...) CLSLog(__FORMAT__, ##__VA_ARGS__)
#endif
#endif

// clang-format off

@implementation EECrashlyticsProtocol

- (id) init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    return self;
}

- (void) causeCrash {
    [[Crashlytics sharedInstance] crash];
}

- (void) causeException {
    [[Crashlytics sharedInstance] throwException];
}

- (void) _log:(NSString* _Nonnull) priorityDescription
          tag:(NSString* _Nonnull) tag
      message:(NSString* _Nonnull) message {
    CLS_LOG(@"%s %s: %s",
            [priorityDescription UTF8String],
            [tag UTF8String],
            [message UTF8String]);
}

- (void) log:(NSDictionary* _Nonnull) dict {
    NSAssert([dict count] == 3, @"...");
    
    NSString* priorityDescription = dict[@"priorityDescription"];
    NSString* tag = dict[@"tag"];
    NSString* message = dict[@"message"];
    
    NSAssert(priorityDescription != nil, @"...");
    NSAssert(tag != nil, @"...");
    NSAssert(message != nil, @"...");
    
    [self _log:priorityDescription tag:tag message:message];
}

@end

// clang-format on