//
//  EEFirebasePerformance.m
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/9/19.
//

#import "ee/firebase_performance/EEFirebasePerformance.h"

#import <FirebasePerformance/FIRPerformance.h>

#import <ee-Swift.h>

#import <ee/core/EEJsonUtils.h>

#import "ee/firebase_performance/EEFirebasePerformanceTrace.h"

@interface EEFirebasePerformance () {
    NSMutableDictionary<NSString*, EEFirebasePerformanceTrace*>* traces_;
}

@end

@implementation EEFirebasePerformance

#define kPrefix @"FirebasePerformanceBridge"

// clang-format off
static NSString* const k__setDataCollectionEnabled  = kPrefix "SetDataCollectionEnabled";
static NSString* const k__isDataCollectionEnabled   = kPrefix "IsDataCollectionEnabled";
static NSString* const k__newTrace                  = kPrefix "NewTrace";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    traces_ = [[NSMutableDictionary alloc] init];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    id<EEIMessageBridge> bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:
        k__setDataCollectionEnabled:^(NSString* message) {
            [self setDataCollectionEnabled:[EEUtils toBool:message]];
            return @"";
        }];

    [bridge registerHandler:
        k__isDataCollectionEnabled:^(NSString* message) {
            return [EEUtils toString:[self isDataCollectionEnabled]];
        }];

    [bridge registerHandler:
                k__newTrace:^(NSString* message) {
                    NSString* traceName = message;
                    return [EEUtils toString:[self newTrace:traceName]];
                }];
}

- (void)deregisterHandlers {
    id<EEIMessageBridge> bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__setDataCollectionEnabled];
    [bridge deregisterHandler:k__isDataCollectionEnabled];
    [bridge deregisterHandler:k__newTrace];
}

- (void)setDataCollectionEnabled:(bool)enabled {
    [FIRPerformance sharedInstance].dataCollectionEnabled = enabled;
}

- (BOOL)isDataCollectionEnabled {
    return [FIRPerformance sharedInstance].dataCollectionEnabled;
}

- (BOOL)newTrace:(NSString*)traceName {
    if ([traces_ objectForKey:traceName] != nil) {
        return NO;
    }
    FIRTrace* trace = [[FIRPerformance sharedInstance] traceWithName:traceName];
    EEFirebasePerformanceTrace* wrapper = [[[EEFirebasePerformanceTrace alloc]
        initWithTraceName:traceName
                    trace:trace] autorelease];
    [traces_ setObject:wrapper forKey:traceName];
    return YES;
}

@end
