//
//  EEUnityAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/unityads/EEUnityAds.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"

#import <UnityAds/UnityAds.h>

@interface EEUnityAds () <UnityAdsDelegate> {
    BOOL playAdSuccessfully_;
}

@end

@implementation EEUnityAds

// clang-format off
NSString* const k__initialize           = @"UnityAds_initialize";
NSString* const k__cppCallback          = @"UnityAds_cppCallback";
NSString* const k__showRewardedVideo    = @"UnityAds_showRewardedVideo";
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
    [UnityAds setDelegate:nil];
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
    [UnityAds initialize:gameId delegate:self];
    // [UnityAds setDebugMode:YES];
}

- (BOOL)isRewardedVideoReady:(NSString*)placementId {
    return [UnityAds isReady:placementId];
}

- (BOOL)showRewardedVideo:(NSString*)placementId {
    if (![self isRewardedVideoReady:placementId]) {
        return NO;
    }
    playAdSuccessfully_ = YES;
    UIViewController* view = [EEUtils getCurrentRootViewController];
    [UnityAds show:view placementId:placementId];
    return playAdSuccessfully_;
}

- (void)unityAdsReady:(NSString*)placementId {
    NSLog(@"%s: placementId = %@", __PRETTY_FUNCTION__, placementId);
}

- (void)unityAdsDidError:(UnityAdsError)error withMessage:(NSString*)message {
    NSLog(@"%s: error %d message %@", __PRETTY_FUNCTION__, (int)error, message);
    playAdSuccessfully_ = NO;
}

- (void)unityAdsDidStart:(NSString*)placementId {
    NSLog(@"%s: placementId = %@", __PRETTY_FUNCTION__, placementId);
}

- (void)unityAdsDidFinish:(NSString*)placementId
          withFinishState:(UnityAdsFinishState)state {
    NSLog(@"%s: placementId = %@ state = %d", __PRETTY_FUNCTION__, placementId,
          (int)state);
    NSMutableDictionary* dict = [NSMutableDictionary dictionary];
    if (state != kUnityAdsFinishStateCompleted) {
        [dict setValue:@(NO) forKey:@"result"];
    } else {
        [dict setValue:@(YES) forKey:@"result"];
    }
    [dict setValue:placementId forKey:@"placementId"];

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__cppCallback
            message:[EEJsonUtils convertDictionaryToString:dict]];
}

@end
