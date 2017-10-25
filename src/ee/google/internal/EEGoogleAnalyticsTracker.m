//
//  EEGoogleAnalyticsTracker.cpp
//  ee_x
//
//  Created by Zinge on 10/24/17.
//
//

#import <GoogleAnalytics/GAI.h>

#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/google/internal/EEGoogleAnalyticsTracker.h"

@interface EEGoogleAnalyticsTracker () {
    NSString* trackingId_;
    id<GAITracker> tracker_;
}

@end

@implementation EEGoogleAnalyticsTracker

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
    [trackingId_ release];
    trackingId_ = nil;
    [tracker_ release];
    [super dealloc];
}

- (NSString* _Nonnull)k__setAllowIDFACollection {
    return [@"GoogleAnalytics_setAllowIDFACollection_"
        stringByAppendingString:trackingId_];
}

- (NSString* _Nonnull)k__send {
    return [@"GoogleAnalytics_send_" stringByAppendingString:trackingId_];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:[self k__setAllowIDFACollection]
                   callback:^(NSString* message) {
                       [self setAllowIDFACollection:[EEUtils toBool:message]];
                       return @"";
                   }];

    [bridge registerHandler:[self k__send]
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       [self send:dict];
                       return @"";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:[self k__setAllowIDFACollection]];
    [bridge deregisterHandler:[self k__send]];
}

- (void)setAllowIDFACollection:(BOOL)enabled {
    [tracker_ setAllowIDFACollection:enabled];
}

- (void)send:(NSDictionary* _Nonnull)dict {
    [tracker_ send:dict];
}

@end
