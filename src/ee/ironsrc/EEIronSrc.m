//
//  EEUnityAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/ironsrc/EEIronSrc.h"

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"

#import "IronSource/IronSource.h"

#undef CLS_LOG
#ifdef __OBJC__
#ifndef NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSNSLog(__FORMAT__, ##__VA_ARGS__)
#else // NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSLog(__FORMAT__, ##__VA_ARGS__)
#endif // NDEBUG
#endif // __OBJC__

@interface EEIronSrc ()<ISRewardedVideoDelegate> {
    
}


@end

@implementation EEIronSrc

NSString* const k__IronSrc_initIronSrc = @"k__IronSrc_initIronSrc";
NSString* const k__IronSrc_isAdsReady = @"k__IronSrc_isAdsReady";
NSString* const k__IronSrc_showAds = @"k__IronSrc_showAds";

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

        [self initIronSrc:GameID];

        return [EEDictionaryUtils emptyResult];
    } tag:k__IronSrc_initIronSrc];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        
        NSString* PlacementID = dict[@"PlacementID"];
        
        return ([self isAdsReady:PlacementID]) ? @"true" : @"false";
    } tag:k__IronSrc_isAdsReady];
    
    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        
        NSString* PlacementID = dict[@"PlacementID"];
        
        [self showAds:PlacementID];
        
        return [EEDictionaryUtils emptyResult];
    } tag:k__IronSrc_showAds];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__IronSrc_initIronSrc];
    [bridge deregisterHandler:k__IronSrc_isAdsReady];
    [bridge deregisterHandler:k__IronSrc_showAds];
}
#pragma mark ===================CODE HERE
- (void) initIronSrc:(NSString*) gameID
{
    _rootController = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    
    [IronSource initWithAppKey:gameID adUnits:@[IS_REWARDED_VIDEO]];
    
    [IronSource setRewardedVideoDelegate:self];    
}

- (BOOL) isAdsReady:(NSString*) placementID
{
    return [IronSource hasRewardedVideo];
}

- (void) showAds:(NSString*) placementID
{
    [IronSource showRewardedVideoWithViewController:_rootController placement:placementID];
}
#pragma mark ===================IronSrcSDKDelegate
- (void)rewardedVideoDidClose
{
    NSLog(@"IronSrc rewardedVideoDidClose");
}

- (void)didReceiveRewardForPlacement:(ISPlacementInfo *)placementInfo
{
    NSLog(@"IronSrc didReceiveRewardForPlacement  %@", placementInfo.placementName);
    
    NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
    [dict setValue:[NSNumber numberWithInteger:2] forKey:@"code"];
    [dict setValue:placementInfo.placementName forKey:@"placement"];
    
    NSLog(@"EEIronSrcADS   finish dict %@", [EEJsonUtils convertDictionaryToString:dict]);
    [[EEMessageBridge getInstance] callCpp:@"__IronSrcAds_callback" msg:[EEJsonUtils convertDictionaryToString:dict]];
}

@end
