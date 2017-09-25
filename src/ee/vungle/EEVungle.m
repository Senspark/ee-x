//
//  EEVungle.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <UIKit/UIKit.h>
#import <VungleSDK/VungleSDK.h>

#import "ee/vungle/EEVungle.h"

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"

@interface EEVungle () <VungleSDKDelegate>

@end

@implementation EEVungle

NSString* const k__VungleAds_initVungleAds = @"k__VungleAds_initVungleAds";
NSString* const k__VungleAds_isAdReady = @"k__VungleAds_isAdReady";
NSString* const k__VungleAds_showAds = @"k__VungleAds_showAds";

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
        
        NSString* PlacementIDs = dict[@"PlacementIDs"];
        
        NSArray *listPlacements = [PlacementIDs componentsSeparatedByString:@"/"];

        [self initVungle:GameID placementIds:listPlacements];

        return [EEDictionaryUtils emptyResult];
    } tag:k__VungleAds_initVungleAds];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* PlacementID = dict[@"PlacementID"];

        return ([self isAdReady:PlacementID]) ? @"true" : @"false";
    } tag:k__VungleAds_isAdReady];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* PlacementID = dict[@"PlacementID"];

        [self showAds:PlacementID];

        return [EEDictionaryUtils emptyResult];
    } tag:k__VungleAds_showAds];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__VungleAds_initVungleAds];
    [bridge deregisterHandler:k__VungleAds_isAdReady];
    [bridge deregisterHandler:k__VungleAds_showAds];
    
    VungleSDK* sdk = [VungleSDK sharedSDK];
    [sdk setDelegate:nil];
}

#pragma mark - CODE HERE

- (void)initVungle:(NSString*)gameID placementIds: (NSArray*) placementArr {
    _rootController = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    VungleSDK* sdk = [VungleSDK sharedSDK];

    [sdk startWithAppId:gameID];
    [sdk setLoggingEnabled:YES];
    
    [sdk setDelegate:self];
    
//    [sdk startWithAppId:gameID placements:placementArr error:nil];
}

- (BOOL)isAdReady:(NSString*)placementID {
    VungleSDK* sdk = [VungleSDK sharedSDK];
    return [sdk isAdPlayable];
//    return [sdk isAdCachedForPlacementID:placementID];
}

- (void)showAds:(NSString*)placementID {
    VungleSDK* sdk = [VungleSDK sharedSDK];
    [sdk playAd:_rootController error:nil];
//    [sdk playAd:_rootController options:nil placementID:placementID error:nil];
}
#pragma mark - VungleSDKDelegate
//- (void)vungleAdPlayabilityUpdate:(BOOL)isAdPlayable placementID:(nullable NSString *)placementID
//{
//    NSLog(@"EEVungle  placement %@ isAdPlayable %d", placementID, isAdPlayable);
//}
//
//- (void)vungleWillCloseAdWithViewInfo:(nonnull VungleViewInfo *)info placementID:(nonnull NSString *)placementID
//{
//    NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
//    [dict setValue:[NSNumber numberWithInteger:2] forKey:@"code"];
//    [dict setValue:placementID forKey:@"placement"];
//    
//    NSLog(@"EEVungle   finish dict %@",
//          [EEJsonUtils convertDictionaryToString:dict]);
//    [[EEMessageBridge getInstance]
//     callCpp:@"__VungleAds_callback"
//     msg:[EEJsonUtils convertDictionaryToString:dict]];
//}

- (void)vungleSDKwillCloseAdWithViewInfo:(NSDictionary *)viewInfo
                 willPresentProductSheet:(BOOL)willPresentProductSheet __attribute__((deprecated("Use vungleSDKWillCloseAdWithViewInfo: instead.")))
{
    NSLog(@"EEVungle  ");
}

/**
 * If implemented, this will get called when the product sheet is about to be closed.
 *
 * @note Because `vungleSDKWillCloseAdWithViewInfo:willPresentProductSheet:` is now called
 * after Product Sheet dismissal, this method should no longer be necessary. It will be
 * removed in a future version.
 */
- (void)vungleSDKwillCloseProductSheet:(id)productSheet __attribute__((deprecated("Use vungleSDKWillCloseAdWithViewInfo: instead.")))
{
    NSLog(@"EEVungle  ");
}

- (void)vungleSDKWillCloseAdWithViewInfo:(NSDictionary *)viewInfo
{
    if ([viewInfo[@"completedView"] boolValue]) {
        NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
        [dict setValue:[NSNumber numberWithInteger:2] forKey:@"code"];
        [dict setValue:@"rewardVideo" forKey:@"placement"];
    
        NSLog(@"EEVungle   finish dict %@",
              [EEJsonUtils convertDictionaryToString:dict]);
        [[EEMessageBridge getInstance]
         callCpp:@"__VungleAds_callback"
         msg:[EEJsonUtils convertDictionaryToString:dict]];
    }
}

- (void)vungleSDKAdPlayableChanged:(BOOL)isAdPlayable
{
    NSLog(@"EEVungle  isAdPlayable %d", isAdPlayable);
}
@end
