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

@interface EEUnityAds () <UnityAdsDelegate>
@end

@implementation EEUnityAds

// clang-format off
static NSString* const k__initialize           = @"UnityAds_initialize";
static NSString* const k__setDebugModeEnabled  = @"UnityAds_setDebugModeEnabled";
static NSString* const k__isRewardedVideoReady = @"UnityAds_isRewardedVideoReady";
static NSString* const k__showRewardedVideo    = @"UnityAds_showRewardedVideo";
static NSString* const k__onError              = @"UnityAds_onError";
static NSString* const k__onSkipped            = @"UnityAds_onSkipped";
static NSString* const k__onFinished           = @"UnityAds_onFinished";
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
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       NSString* gameId = dict[@"gameId"];
                       BOOL testModeEnabled =
                           [(NSString*)dict[@"testModeEnabled"]
                               isEqualToString:@"true"];
                       [self initialize:gameId testMode:testModeEnabled];
                       return @"";
                   }];

    [bridge registerHandler:k__setDebugModeEnabled
                   callback:^(NSString* message) {
                       [self setDebugMode:[message isEqualToString:@"true"]];
                       return @"";
                   }];

    [bridge registerHandler:k__isRewardedVideoReady
                   callback:^(NSString* message) {
                       NSString* placementId = message;
                       return [self isRewardedVideoReady:placementId]
                                  ? @"true"
                                  : @"false";
                   }];

    [bridge registerHandler:k__showRewardedVideo
                   callback:^(NSString* message) {
                       NSString* placementId = message;
                       [self showRewardedVideo:placementId];
                       return @"";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__initialize];
    [bridge deregisterHandler:k__setDebugModeEnabled];
    [bridge deregisterHandler:k__isRewardedVideoReady];
    [bridge deregisterHandler:k__showRewardedVideo];
}

- (void)initialize:(NSString* _Nonnull)gameId testMode:(BOOL)testModeEnabled {
    [UnityAds initialize:gameId delegate:self testMode:testModeEnabled];
}

- (void)setDebugMode:(BOOL)enabled {
    [UnityAds setDebugMode:enabled];
}

- (BOOL)isRewardedVideoReady:(NSString*)placementId {
    return [UnityAds isReady:placementId];
}

- (void)showRewardedVideo:(NSString*)placementId {
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [UnityAds show:rootView placementId:placementId];
}

- (void)unityAdsReady:(NSString*)placementId {
    NSLog(@"%s: placementId = %@", __PRETTY_FUNCTION__, placementId);
}

- (void)unityAdsDidError:(UnityAdsError)error withMessage:(NSString*)message {
    NSLog(@"%s: error %d message %@", __PRETTY_FUNCTION__, (int)error, message);
}

- (void)unityAdsDidStart:(NSString*)placementId {
    NSLog(@"%s: placementId = %@", __PRETTY_FUNCTION__, placementId);
}

- (void)unityAdsDidFinish:(NSString*)placementId
          withFinishState:(UnityAdsFinishState)state {
    NSLog(@"%s: placementId = %@ state = %d", __PRETTY_FUNCTION__, placementId,
          (int)state);

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    if (state == kUnityAdsFinishStateError) {
        [bridge callCpp:k__onError message:placementId];
        return;
    }
    if (state == kUnityAdsFinishStateSkipped) {
        [bridge callCpp:k__onSkipped message:placementId];
        return;
    }
    if (state == kUnityAdsFinishStateCompleted) {
        [bridge callCpp:k__onFinished message:placementId];
        return;
    }
    NSAssert(NO, @"");
}

@end
