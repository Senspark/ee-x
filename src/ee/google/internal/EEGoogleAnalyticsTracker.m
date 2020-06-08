//
//  EEGoogleAnalyticsTracker.cpp
//  ee_x
//
//  Created by Zinge on 10/24/17.
//
//

#import <GoogleAnalytics/GAI.h>

#import <ee_x-Swift.h>

#import <ee/core/internal/EEJsonUtils.h>

#import "ee/google/internal/EEGoogleAnalyticsTracker.h"

@interface EEGoogleAnalyticsTracker () {
    NSString* trackingId_;
    id<GAITracker> tracker_;
}

@end

@implementation EEGoogleAnalyticsTracker

// clang-format off
static NSString* const k__key   = @"key";
static NSString* const k__value = @"value";
// clang-format on

- (id _Nullable)initWithTrackingId:(NSString* _Nonnull)trackingId {
    self = [super init];
    if (self == nil) {
        return self;
    }

    trackingId_ = [trackingId copy];
    id<GAITracker> tracker =
        [[GAI sharedInstance] trackerWithName:trackingId trackingId:trackingId];
    tracker_ = [tracker retain];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [[GAI sharedInstance] removeTrackerByName:trackingId_];
    [trackingId_ release];
    trackingId_ = nil;
    [tracker_ release];
    [super dealloc];
}

- (NSString* _Nonnull)k__setParameter {
    return
        [@"GoogleAnalytics_setParameter_" stringByAppendingString:trackingId_];
}

- (NSString* _Nonnull)k__setAllowIDFACollection {
    return [@"GoogleAnalytics_setAllowIDFACollection_"
        stringByAppendingString:trackingId_];
}

- (NSString* _Nonnull)k__send {
    return [@"GoogleAnalytics_send_" stringByAppendingString:trackingId_];
}

- (void)registerHandlers {
    id<EEIMessageBridge> bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:[self k__setParameter]:^(NSString* message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* key = [dict objectForKey:k__key];
        NSString* value = [dict objectForKey:k__value];
        [self setParameter:key value:value];
        return @"";
    }];

    [bridge
        registerHandler:[self k__setAllowIDFACollection]:^(NSString* message) {
            [self setAllowIDFACollection:[EEUtils toBool:message]];
            return @"";
        }];

    [bridge registerHandler:[self k__send]:^(NSString* message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        [self send:dict];
        return @"";
    }];
}

- (void)deregisterHandlers {
    id<EEIMessageBridge> bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:[self k__setParameter]];
    [bridge deregisterHandler:[self k__setAllowIDFACollection]];
    [bridge deregisterHandler:[self k__send]];
}

- (void)setParameter:(NSString* _Nonnull)key value:(NSString* _Nonnull)value {
    [tracker_ set:key value:value];
}

- (void)setAllowIDFACollection:(BOOL)enabled {
    [tracker_ setAllowIDFACollection:enabled];
}

- (void)send:(NSDictionary* _Nonnull)dict {
    [tracker_ send:dict];
}

@end
