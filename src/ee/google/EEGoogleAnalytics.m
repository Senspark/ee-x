//
//  EEGoogleAnalytics.cpp
//  ee_x
//
//  Created by Zinge on 10/24/17.
//
//

#import <GoogleAnalytics/GAI.h>
#import <GoogleAnalytics/GAIDictionaryBuilder.h>
#import <GoogleAnalytics/GAIECommerceFields.h>
#import <GoogleAnalytics/GAIFields.h>

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/google/EEGoogleAnalytics.h"
#import "ee/google/internal/EEGoogleAnalyticsTracker.h"

@interface EEGoogleAnalytics () {
    NSMutableDictionary<NSString*, EEGoogleAnalyticsTracker*>* trackers_;
}

@end

@implementation EEGoogleAnalytics

// clang-format off
static NSString* const k__setDispatchInterval          = @"GoogleAnalytics_setDispatchInterval";
static NSString* const k__setDryRun                    = @"GoogleAnalytics_setDryRun";
static NSString* const k__setOptOut                    = @"GoogleAnalytics_setOptOut";
static NSString* const k__setTrackUncaughtException    = @"GoogleAnalytics_setTrackUncaughtException";
static NSString* const k__dispatch                     = @"GoogleAnalytics_dispatch";
static NSString* const k__createTracker                = @"GoogleAnalytics_createTracker";
static NSString* const k__destroyTracker               = @"GoogleAnalytics_destroyTracker";

static NSString* const k__testTrackEvent               = @"GoogleAnalytics_testTrackEvent";
static NSString* const k__testTrackException           = @"GoogleAnalytics_testTrackException";
static NSString* const k__testTrackScreenView          = @"GoogleAnalytics_testTrackScreenView";
static NSString* const k__testTrackSocial              = @"GoogleAnalytics_testTrackSocial";
static NSString* const k__testTrackTiming              = @"GoogleAnalytics_testTrackTiming";
static NSString* const k__testCustomDimensionAndMetric = @"GoogleAnalytics_testCustomDimensionAndMetric";
static NSString* const k__testTrackEcommerceAction     = @"GoogleAnalytics_testTrackEcommerceAction";
static NSString* const k__testTrackEcommerceImpression = @"GoogleAnalytics_testTrackEcommerceImpression";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    trackers_ = [[NSMutableDictionary alloc] init];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [trackers_ release];
    trackers_ = nil;
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:k__setDispatchInterval
                   callback:^(NSString* message) {
                       NSTimeInterval seconds = [message intValue];
                       [self setDispatchInterval:seconds];
                       return @"";
                   }];

    [bridge registerHandler:k__setDryRun
                   callback:^(NSString* message) {
                       [self setDryRun:[EEUtils toBool:message]];
                       return @"";
                   }];

    [bridge registerHandler:k__setOptOut
                   callback:^(NSString* message) {
                       [self setOptOut:[EEUtils toBool:message]];
                       return @"";
                   }];

    [bridge
        registerHandler:k__setTrackUncaughtException
               callback:^(NSString* message) {
                   [self setTrackUncaughtException:[EEUtils toBool:message]];
                   return @"";
               }];

    [bridge registerHandler:k__dispatch
                   callback:^(NSString* message) {
                       [self dispatch];
                       return @"";
                   }];

    [bridge registerHandler:k__createTracker
                   callback:^(NSString* message) {
                       NSString* trackingId = message;
                       return
                           [EEUtils toString:[self createTracker:trackingId]];
                   }];

    [bridge registerHandler:k__destroyTracker
                   callback:^(NSString* message) {
                       NSString* trackingId = message;
                       return
                           [EEUtils toString:[self destroyTracker:trackingId]];
                   }];

    [bridge registerHandler:k__testTrackEvent
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       return [EEUtils toString:[self testTrackEvent:dict]];
                   }];

    [bridge registerHandler:k__testTrackException
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       return [EEUtils toString:[self testTrackException:dict]];
                   }];

    [bridge registerHandler:k__testTrackScreenView
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       return
                           [EEUtils toString:[self testTrackScreenView:dict]];
                   }];

    [bridge registerHandler:k__testTrackSocial
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       return [EEUtils toString:[self testTrackSocial:dict]];
                   }];

    [bridge registerHandler:k__testTrackTiming
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       return [EEUtils toString:[self testTrackTiming:dict]];
                   }];

    [bridge registerHandler:k__testCustomDimensionAndMetric
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       return [EEUtils
                           toString:[self testCustomDimensionAndMetric:dict]];
                   }];

    [bridge registerHandler:k__testTrackEcommerceAction
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       return [EEUtils
                           toString:[self testTrackEcommerceAction:dict]];
                   }];

    [bridge registerHandler:k__testTrackEcommerceImpression
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       return [EEUtils
                           toString:[self testTrackEcommerceImpression:dict]];
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__setDispatchInterval];
    [bridge deregisterHandler:k__setDryRun];
    [bridge deregisterHandler:k__setOptOut];
    [bridge deregisterHandler:k__setTrackUncaughtException];
    [bridge deregisterHandler:k__dispatch];
    [bridge deregisterHandler:k__createTracker];
    [bridge deregisterHandler:k__destroyTracker];

    [bridge deregisterHandler:k__testTrackEvent];
    [bridge deregisterHandler:k__testTrackException];
    [bridge deregisterHandler:k__testTrackScreenView];
    [bridge deregisterHandler:k__testTrackSocial];
    [bridge deregisterHandler:k__testTrackTiming];
    [bridge deregisterHandler:k__testTrackEcommerceAction];
    [bridge deregisterHandler:k__testTrackEcommerceImpression];
}

- (void)setDispatchInterval:(NSTimeInterval)interval {
    [[GAI sharedInstance] setDispatchInterval:interval];
}

- (void)setDryRun:(BOOL)enabled {
    [[GAI sharedInstance] setDryRun:enabled];
}

- (void)setOptOut:(BOOL)enabled {
    [[GAI sharedInstance] setOptOut:enabled];
}

- (void)setTrackUncaughtException:(BOOL)enabled {
    [[GAI sharedInstance] setTrackUncaughtExceptions:enabled];
}

- (void)dispatch {
    [[GAI sharedInstance] dispatch];
}

- (BOOL)createTracker:(NSString* _Nonnull)trackingId {
    if ([trackers_ objectForKey:trackingId] != nil) {
        return NO;
    }
    EEGoogleAnalyticsTracker* tracker = [[[EEGoogleAnalyticsTracker alloc]
        initWithTrackingId:trackingId] autorelease];
    [trackers_ setObject:tracker forKey:trackingId];
    return YES;
}

- (BOOL)destroyTracker:(NSString* _Nonnull)trackingId {
    if ([trackers_ objectForKey:trackingId] == nil) {
        return NO;
    }
    [trackers_ removeObjectForKey:trackingId];
    return YES;
}

- (BOOL)checkDictionary:(NSDictionary* _Nonnull)builtDict
                   dict:(NSDictionary* _Nonnull)expectedDict {
    if ([builtDict count] != [expectedDict count]) {
        NSLog(@"Dictionary size mismatched: expected %d found %d",
              (int)([expectedDict count]), (int)([builtDict count]));
        return NO;
    }
    BOOL matched = YES;
    NSArray* allKeys = [expectedDict allKeys];
    for (NSString* key in allKeys) {
        NSString* expectedValue = [expectedDict valueForKey:key];
        NSString* value = [builtDict valueForKey:key];
        if (![value isEqualToString:expectedValue]) {
            NSLog(@"Element value mismatched: expected %@ found %@",
                  expectedValue, value);
            matched = NO;
        }
    }
    return matched;
}

- (BOOL)testTrackEvent:(NSDictionary* _Nonnull)dict {
    NSDictionary* expectedDict =
        [[GAIDictionaryBuilder createEventWithCategory:@"category"
                                                action:@"action"
                                                 label:@"label"
                                                 value:@(1)] build];
    return [self checkDictionary:dict dict:expectedDict];
}

- (BOOL)testTrackException:(NSDictionary* _Nonnull)dict {
    NSDictionary* expectedDict =
        [[GAIDictionaryBuilder createExceptionWithDescription:@"description"
                                                    withFatal:@(YES)] build];
    return [self checkDictionary:dict dict:expectedDict];
}

- (BOOL)testTrackScreenView:(NSDictionary* _Nonnull)dict {
    NSDictionary* expectedDict =
        [[GAIDictionaryBuilder createScreenView] build];
    return [self checkDictionary:dict dict:expectedDict];
}

- (BOOL)testTrackSocial:(NSDictionary* _Nonnull)dict {
    NSDictionary* expectedDict =
        [[GAIDictionaryBuilder createSocialWithNetwork:@"network"
                                                action:@"action"
                                                target:@"target"] build];
    return [self checkDictionary:dict dict:expectedDict];
}

- (BOOL)testTrackTiming:(NSDictionary* _Nonnull)dict {
    NSDictionary* expectedDict =
        [[GAIDictionaryBuilder createTimingWithCategory:@"category"
                                               interval:@(1)
                                                   name:@"variable"
                                                  label:@"label"] build];
    return [self checkDictionary:dict dict:expectedDict];
}

- (BOOL)testCustomDimensionAndMetric:(NSDictionary* _Nonnull)dict {
    GAIDictionaryBuilder* builder = [GAIDictionaryBuilder createScreenView];
    [builder set:@"1" forKey:[GAIFields customMetricForIndex:1]];
    [builder set:@"2" forKey:[GAIFields customMetricForIndex:2]];
    [builder set:@"5.5" forKey:[GAIFields customMetricForIndex:5]];
    [builder set:@"dimension_1" forKey:[GAIFields customDimensionForIndex:1]];
    [builder set:@"dimension_2" forKey:[GAIFields customDimensionForIndex:2]];

    NSDictionary* expectedDict = [builder build];
    return [self checkDictionary:dict dict:expectedDict];
}

- (BOOL)testTrackEcommerceAction:(NSDictionary* _Nonnull)dict {
    GAIEcommerceProduct* product0 =
        [[[GAIEcommerceProduct alloc] init] autorelease];
    [product0 setCategory:@"category0"];
    [product0 setId:@"id0"];
    [product0 setName:@"name0"];
    [product0 setPrice:@(1.5)];

    GAIEcommerceProduct* product1 =
        [[[GAIEcommerceProduct alloc] init] autorelease];
    [product1 setCategory:@"category1"];
    [product1 setId:@"id1"];
    [product1 setName:@"name1"];
    [product1 setPrice:@(2)];

    GAIEcommerceProductAction* action =
        [[[GAIEcommerceProductAction alloc] init] autorelease];
    [action setAction:kGAIPAPurchase];
    [action setProductActionList:@"actionList"];
    [action setProductListSource:@"listSource"];
    [action setTransactionId:@"transactionId"];
    [action setRevenue:@(2.0)];

    GAIDictionaryBuilder* builder = [GAIDictionaryBuilder createScreenView];
    [builder addProduct:product0];
    [builder addProduct:product1];
    [builder setProductAction:action];

    NSDictionary* expectedDict = [builder build];
    return [self checkDictionary:dict dict:expectedDict];
}

- (BOOL)testTrackEcommerceImpression:(NSDictionary* _Nonnull)dict {
    GAIEcommerceProduct* product0 =
        [[[GAIEcommerceProduct alloc] init] autorelease];
    [product0 setCategory:@"category0"];
    [product0 setId:@"id0"];
    [product0 setName:@"name0"];
    [product0 setPrice:@(1.5)];

    GAIEcommerceProduct* product1 =
        [[[GAIEcommerceProduct alloc] init] autorelease];
    [product1 setCategory:@"category1"];
    [product1 setId:@"id1"];
    [product1 setName:@"name1"];
    [product1 setPrice:@(2.5)];

    GAIEcommerceProduct* product2 =
        [[[GAIEcommerceProduct alloc] init] autorelease];
    [product2 setCategory:@"category2"];
    [product2 setId:@"id2"];
    [product2 setName:@"name2"];
    [product2 setPrice:@(3.0)];

    GAIEcommerceProduct* product3 =
        [[[GAIEcommerceProduct alloc] init] autorelease];
    [product3 setCategory:@"category3"];
    [product3 setId:@"id3"];
    [product3 setName:@"name3"];
    [product3 setPrice:@(4)];

    GAIDictionaryBuilder* builder = [GAIDictionaryBuilder createScreenView];
    [builder addProductImpression:product0
                   impressionList:@"impressionList0"
                 impressionSource:nil];
    [builder addProductImpression:product1
                   impressionList:@"impressionList0"
                 impressionSource:nil];
    [builder addProductImpression:product2
                   impressionList:@"impressionList1"
                 impressionSource:nil];
    [builder addProductImpression:product3
                   impressionList:@"impressionList1"
                 impressionSource:nil];

    NSDictionary* expectedDict = [builder build];
    return [self checkDictionary:dict dict:expectedDict];
}

@end
