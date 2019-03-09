//
//  EEFirebasePerformance.m
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/9/19.
//

#import "EEFirebasePerformance.h"

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"


@interface EEFirebasePerformance () {
//    NSMutableDictionary<NSString*, EE
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
    
}

- (BOOL)isDataCollectionEnabled {
    return NO;
}

- (BOOL) newTrace:(NSString *)traceName {
    return NO;
}

- (BOOL) startTrace:(NSString *)traceName {
    return NO;
}

@end
