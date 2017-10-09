//
//  EEUnityAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <IronSource/IronSource.h>
#import <UIKit/UIKit.h>

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/ironsource/EEIronSource.h"

@interface EEIronSource () <ISRewardedVideoDelegate>
@property (nonatomic, retain) NSString* _Nullable currentPlacementId;
@end

@implementation EEIronSource

// clang-format off
static NSString* const k__initialize        = @"IronSource_initialize";
static NSString* const k__cppCallback       = @"IronSource_cppCallback";
static NSString* const k__showRewardedVideo = @"IronSource_showRewardedVideo";
// clang-format on

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
    [self setCurrentPlacementId:nil];
    [IronSource setRewardedVideoDelegate:nil];
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:k__initialize
                   callback:^(NSString* message) {
                       NSString* gameId = message;
                       [self initialize:gameId];
                       return @"";
                   }];

    [bridge registerHandler:k__showRewardedVideo
                   callback:^(NSString* message) {
                       NSString* placementId = message;
                       return [self showRewardedVideo:placementId] ? @"true"
                                                                   : @"false";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__initialize];
    [bridge deregisterHandler:k__showRewardedVideo];
}

- (void)initialize:(NSString*)gameId {
    [IronSource initWithAppKey:gameId adUnits:@[IS_REWARDED_VIDEO]];
    [IronSource setRewardedVideoDelegate:self];
}

- (BOOL)isRewardedVideoReady {
    return [IronSource hasRewardedVideo];
}

- (BOOL)showRewardedVideo:(NSString* _Nonnull)placementId {
    if (![self isRewardedVideoReady]) {
        return NO;
    }
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [self setCurrentPlacementId:placementId];
    [IronSource showRewardedVideoWithViewController:rootView
                                          placement:placementId];
    return YES;
}

- (void)sendResult:(BOOL)result placementId:(NSString* _Nonnull)placementId {
    NSMutableDictionary* dict = [NSMutableDictionary dictionary];
    [dict setValue:@(result) forKey:@"result"];
    [dict setValue:placementId forKey:@"placementId"];

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__cppCallback
            message:[EEJsonUtils convertDictionaryToString:dict]];
}

- (void)rewardedVideoHasChangedAvailability:(BOOL)available {
    NSLog(@"%s: %d", __PRETTY_FUNCTION__, (int)available);
}

- (void)didReceiveRewardForPlacement:(ISPlacementInfo*)placementInfo {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [placementInfo placementName]);
    [self sendResult:YES placementId:[placementInfo placementName]];
}

- (void)rewardedVideoDidFailToShowWithError:(NSError*)error {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [self sendResult:NO placementId:[self currentPlacementId]];
    [self setCurrentPlacementId:nil];
}

- (void)rewardedVideoDidOpen {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)rewardedVideoDidClose {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)rewardedVideoDidStart {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)rewardedVideoDidEnd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

@end
