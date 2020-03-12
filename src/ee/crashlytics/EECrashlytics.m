//
//  EECrashlyticsProtocol.m
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/crashlytics/EECrashlytics.h"

#import <Crashlytics/Crashlytics.h>
#import <Fabric/Fabric.h>

#import <ee/core/internal/EEDictionaryUtils.h>
#import <ee/core/internal/EEJsonUtils.h>
#import <ee/core/internal/EEMessageBridge.h>

#undef CLS_LOG
#ifdef __OBJC__
#ifndef NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSNSLog(__FORMAT__, ##__VA_ARGS__)
#else // NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSLog(__FORMAT__, ##__VA_ARGS__)
#endif // NDEBUG
#endif // __OBJC__

@implementation EECrashlytics {
    EEMessageBridge* bridge_;
}

// clang-format off
NSString* const k__crashlytics_cause_crash     = @"__crashlytics_cause_crash";
NSString* const k__crashlytics_cause_exception = @"__crashlytics_cause_exception";
NSString* const k__crashlytics_set_log_level   = @"__crashlytics_set_log_level";
NSString* const k__crashlytics_log             = @"__crashlytics_log";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }

    // Initialized automatically.
    // https://firebase.google.com/docs/crashlytics/upgrade-from-crash-reporting#ios
    // [Fabric with:@[[Crashlytics class], [Answers class]]];

    bridge_ = [EEMessageBridge getInstance];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:^(NSString* msg) {
        [self causeCrash];
        return @"";
    }
                         tag:k__crashlytics_cause_crash];

    [bridge_ registerHandler:^(NSString* msg) {
        [self causeException];
        return @"";
    }
                         tag:k__crashlytics_cause_exception];

    [bridge_ registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 1, @"...");

        // Ignored.

        return @"";
    }
                         tag:k__crashlytics_set_log_level];

    [bridge_ registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 4, @"...");

        NSString* priorityDescription = dict[@"priorityDescription"];
        NSString* tag = dict[@"tag"];
        NSString* message = dict[@"message"];

        NSAssert(priorityDescription != nil, @"...");
        NSAssert(tag != nil, @"...");
        NSAssert(message != nil, @"...");

        [self log:priorityDescription tag:tag message:message];
        return @"";
    }
                         tag:k__crashlytics_log];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__crashlytics_cause_crash];
    [bridge_ deregisterHandler:k__crashlytics_cause_exception];
    [bridge_ deregisterHandler:k__crashlytics_log];
}

- (void)causeCrash {
    [[Crashlytics sharedInstance] crash];
}

- (void)causeException {
    [[Crashlytics sharedInstance] throwException];
}

- (void)log:(NSString* _Nonnull)desc
        tag:(NSString* _Nonnull)tag
    message:(NSString* _Nonnull)message {
    CLS_LOG(@"%s %s: %s", [desc UTF8String], [tag UTF8String],
            [message UTF8String]);
}

@end
