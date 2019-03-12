//
//  EEFirebasePerformance.m
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/9/19.
//

#import "EEFirebasePerformance.h"

#import <FirebaseCore/FIRApp.h>
#import <FirebasePerformance/FIRPerformance.h>

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/firebase/internal/EEFirebasePerformanceTrace.h"


@interface EEFirebasePerformance () {
    NSMutableDictionary<NSString*, EEFirebasePerformanceTrace*>* traces_;
}
@end

@implementation EEFirebasePerformance

// clang-format off
static NSString* const k__setDataCollectionEnabled  = @"FirebasePerformance_setDataCollectionEnabled";
static NSString* const k__isDataCollectionEnabled   = @"FirebasePerformance_isDataCollectionEnabled";
static NSString* const k__startTrace                = @"FirebasePerformance_startTrace";
static NSString* const k__newTrace                  = @"FirebasePerformance_newTrace";
// clang-format on

- (id) init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    
    [FIRApp configure];
    
    traces_ = [[NSMutableDictionary alloc] init];
    [self registerHandlers];
    return self;
}

- (void) dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void) registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    
    [bridge registerHandler:k__setDataCollectionEnabled callback:^(NSString* message) {
        [self setDataCollectionEnabled:[EEUtils toBool:message]];
        return @"";
    }];
    
    [bridge registerHandler:k__isDataCollectionEnabled callback:^(NSString* message) {
        return [EEUtils toString:[self isDataCollectionEnabled]];
    }];
    
    [bridge registerHandler:k__startTrace callback:^(NSString* message) {
        NSString* traceName = message;
        return [EEUtils toString:[self startTrace:traceName]];
    }];
    
    [bridge registerHandler:k__newTrace callback:^(NSString* message) {
        NSString* traceName = message;
        return [EEUtils toString:[self newTrace:traceName]];
    }];
}

- (void) deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    
    [bridge deregisterHandler:k__setDataCollectionEnabled];
    [bridge deregisterHandler:k__isDataCollectionEnabled];
    [bridge deregisterHandler:k__startTrace];
    [bridge deregisterHandler:k__newTrace];
}

- (void) setDataCollectionEnabled:(bool)enabled {
    [FIRPerformance sharedInstance].dataCollectionEnabled = enabled;
}

- (BOOL)isDataCollectionEnabled {
    return [FIRPerformance sharedInstance].dataCollectionEnabled;
}

- (BOOL) newTrace:(NSString *)traceName {
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

- (BOOL) startTrace:(NSString *)traceName {
    if ([traces_ objectForKey:traceName] != nil) {
        return NO;
    }
    FIRTrace* trace = [FIRPerformance startTraceWithName:traceName];
    EEFirebasePerformanceTrace* wrapper = [[[EEFirebasePerformanceTrace alloc]
        initWithTraceName:traceName
                    trace:trace] autorelease];
    [traces_ setObject:wrapper forKey:traceName];
    return YES;
}

@end
