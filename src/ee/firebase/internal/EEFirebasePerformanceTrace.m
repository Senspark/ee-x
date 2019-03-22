//
//  EEFirebasePerformanceTrace.m
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/9/19.
//

#import "ee/firebase/internal/EEFirebasePerformanceTrace.h"

#import <FirebasePerformance/FIRPerformance.h>
#import <FirebasePerformance/FIRTrace.h>

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"

@interface EEFirebasePerformanceTrace () {
    NSString* traceName_;
    FIRTrace* trace_;
}

@end

@implementation EEFirebasePerformanceTrace

// clang-format off
static NSString* const k__key = @"key";
static NSString* const k__value = @"value";
// clang-format on

- (id _Nullable)initWithTraceName:(NSString* _Nonnull)traceName
                            trace:(FIRTrace*)trace {
    self = [super init];
    if (self == nil) {
        return self;
    }

    traceName_ = [traceName copy];
    trace_ = trace;
    [trace_ retain];

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [traceName_ release];
    traceName_ = nil;
    [trace_ release];
    [super dealloc];
}

- (NSString* _Nonnull)k__start {
    return [@"FirebasePerformance_start_" stringByAppendingString:traceName_];
}

- (NSString* _Nonnull)k__stop {
    return [@"FirebasePerformance_stop_" stringByAppendingString:traceName_];
}

- (NSString* _Nonnull)k__incrementMetric {
    return [@"FirebasePerformance_incrementMetric_"
        stringByAppendingString:traceName_];
}

- (NSString* _Nonnull)k__getLongMetric {
    return [@"FirebasePerformance_getLongMetric_"
        stringByAppendingString:traceName_];
}

- (NSString* _Nonnull)k__putMetric {
    return
        [@"FirebasePerformance_putMetric_" stringByAppendingString:traceName_];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:[self k__start]
                   callback:^(NSString* message) {
                       [self start];
                       return @"";
                   }];

    [bridge registerHandler:[self k__stop]
                   callback:^(NSString* message) {
                       [self stop];
                       return @"";
                   }];

    [bridge registerHandler:[self k__putMetric]
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       NSString* name = [dict objectForKey:k__key];
                       int64_t value =
                           [[dict objectForKey:k__value] longLongValue];
                       [self putMetric:name value:value];
                       return @"";
                   }];

    [bridge registerHandler:[self k__incrementMetric]
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       NSString* name = [dict objectForKey:k__key];
                       int64_t value =
                           [[dict objectForKey:k__value] longLongValue];
                       [self incrementMetric:name value:value];
                       return @"";
                   }];

    [bridge registerHandler:[self k__getLongMetric]
                   callback:^(NSString* message) {
                       NSString* name = message;
                       return [EEUtils toString:[self getLongMetric:name]];
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:[self k__start]];
    [bridge deregisterHandler:[self k__stop]];
    [bridge deregisterHandler:[self k__putMetric]];
    [bridge deregisterHandler:[self k__incrementMetric]];
    [bridge deregisterHandler:[self k__getLongMetric]];
}

- (void)start {
    [trace_ start];
}

- (void)stop {
    [trace_ stop];
}

- (void)putMetric:(NSString* _Nonnull)metricName value:(int64_t)value {
    [trace_ setIntValue:value forMetric:metricName];
}

- (void)incrementMetric:(NSString* _Nonnull)metricName value:(int64_t)value {
    [trace_ incrementMetric:metricName byInt:value];
}

- (int64_t)getLongMetric:(NSString* _Nonnull)metricName {
    return [trace_ valueForIntMetric:metricName];
}

@end
