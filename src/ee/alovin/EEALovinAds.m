//
//  EEALovinAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/ALovinads/EEALovinAds.h"

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"

#import <ALovinAds/ALovinAds.h>

@interface EEALovinAds () <ALovinAdsDelegate> {
}

@end

@implementation EEALovinAds

NSString* const k__ALovinads_initALovinAds = @"k__ALovinads_initALovinAds";
NSString* const k__ALovinads_isAdsReady = @"k__ALovinads_isAdsReady";
NSString* const k__ALovinads_showAds = @"k__ALovinads_showAds";

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* GameID = dict[@"GameID"];

        [self initALovinAds:GameID];

        return [EEDictionaryUtils emptyResult];
    } tag:k__ALovinads_initALovinAds];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* PlacementID = dict[@"PlacementID"];

        return ([self isAdsReady:PlacementID]) ? @"true" : @"false";
    } tag:k__ALovinads_isAdsReady];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* PlacementID = dict[@"PlacementID"];

        [self showAds:PlacementID];

        return [EEDictionaryUtils emptyResult];
    } tag:k__ALovinads_showAds];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__ALovinads_initALovinAds];
    [bridge deregisterHandler:k__ALovinads_isAdsReady];
    [bridge deregisterHandler:k__ALovinads_showAds];
}
#pragma mark ===================CODE HERE
- (void)initALovinAds:(NSString*)gameID {
    [ALovinAds initialize:gameID delegate:self];
    //    [ALovinAds setDebugMode:YES];

    _rootController =
        [[[UIApplication sharedApplication] keyWindow] rootViewController];
}

- (BOOL)isAdsReady:(NSString*)placementID {
    return [ALovinAds isReady:placementID];
}

- (void)showAds:(NSString*)placementID {
    if ([self isAdsReady:placementID]) {
        [ALovinAds show:_rootController placementId:placementID];
    }
}
#pragma mark ===================ALovinAdsDelegate
- (void)ALovinAdsReady:(NSString*)placementId {
    NSLog(@"EEALovin ADS   ready %@", placementId);
}

- (void)ALovinAdsDidError:(ALovinAdsError)error withMessage:(NSString*)message {
    NSLog(@"EEALovin ADS   error %@", message);
    NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
    [dict setValue:[NSNumber numberWithInteger:0] forKey:@"code"];
    [dict setValue:message forKey:@"placement"];

    NSLog(@"EEALovin ADS   error dict %@",
          [EEJsonUtils convertDictionaryToString:dict]);
    [[EEMessageBridge getInstance]
        callCpp:@"__ALovinAds_callback"
            msg:[EEJsonUtils convertDictionaryToString:dict]];
}

- (void)ALovinAdsDidStart:(NSString*)placementId {
}

- (void)ALovinAdsDidFinish:(NSString*)placementId
          withFinishState:(ALovinAdsFinishState)state {
    NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
    [dict setValue:[NSNumber numberWithInteger:state] forKey:@"code"];
    [dict setValue:placementId forKey:@"placement"];

    NSLog(@"EEALovin ADS   finish dict %@",
          [EEJsonUtils convertDictionaryToString:dict]);
    [[EEMessageBridge getInstance]
        callCpp:@"__ALovinAds_callback"
            msg:[EEJsonUtils convertDictionaryToString:dict]];
}
@end
