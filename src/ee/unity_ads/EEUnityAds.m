//
//  EEUnityAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/unity_ads/EEUnityAds.h"

#ifdef EE_X_USE_IRON_SOURCE_MEDIATION
#import <ISUnityAdsAdapter/UnityAds.h>
#else // EE_X_USE_IRON_SOURCE_MEDIATION
#import <UnityAds/UnityAds.h>
#endif // EE_X_USE_IRON_SOURCE_MEDIATION

#import <ee_x-Swift.h>

#import <ee/core/internal/EEJsonUtils.h>

#define kPrefix @"UnityAds"

// clang-format off
static NSString* const k__initialize          = kPrefix "_initialize";
static NSString* const k__setDebugModeEnabled = kPrefix "_setDebugModeEnabled";

static NSString* const k__hasRewardedAd       = kPrefix "_hasRewardedAd";
static NSString* const k__showRewardedAd      = kPrefix "_showRewardedAd";

static NSString* const k__onLoaded            = kPrefix "_onLoaded";
static NSString* const k__onFailedToShow      = kPrefix "_onFailedToShow";
static NSString* const k__onClosed            = kPrefix "_onClosed";
// clang-format on

#undef kPrefix

@interface EEUnityAds () <UnityAdsDelegate>
@end

@implementation EEUnityAds {
    id<EEIMessageBridge> bridge_;
    BOOL initialized_;
    BOOL displayed_;
}

- (id)init {
    NSAssert([EEThread isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = [EEMessageBridge getInstance];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)destroy {
    NSAssert([EEThread isMainThread], @"");
    [self deregisterHandlers];
    if (!initialized_) {
        return;
    }
    [UnityAds removeDelegate:self];
}

- (void)registerHandlers {
    [bridge_ registerHandler:
               k__initialize:^(NSString* message) {
                   NSDictionary* dict =
                       [EEJsonUtils convertStringToDictionary:message];
                   NSString* gameId = dict[@"gameId"];
                   BOOL testModeEnabled = [dict[@"testModeEnabled"] boolValue];
                   [self initialize:gameId testMode:testModeEnabled];
                   return @"";
               }];

    [bridge_ registerHandler:
        k__setDebugModeEnabled:^(NSString* message) {
            [self setDebugMode:[EEUtils toBool:message]];
            return @"";
        }];

    [bridge_ registerHandler:
            k__hasRewardedAd:^(NSString* message) {
                NSString* adId = message;
                return [EEUtils toString:[self hasRewardedAd:adId]];
            }];

    [bridge_ registerHandler:
           k__showRewardedAd:^(NSString* message) {
               NSString* adId = message;
               [self showRewardedAd:adId];
               return @"";
           }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__initialize];
    [bridge_ deregisterHandler:k__setDebugModeEnabled];
    [bridge_ deregisterHandler:k__hasRewardedAd];
    [bridge_ deregisterHandler:k__showRewardedAd];
}

- (void)initialize:(NSString* _Nonnull)gameId testMode:(BOOL)testModeEnabled {
    NSAssert([EEThread isMainThread], @"");
    if (initialized_) {
        return;
    }
    if (![UnityAds isSupported]) {
        return;
    }
    [UnityAds initialize:gameId testMode:testModeEnabled];
    [UnityAds addDelegate:self];
    initialized_ = YES;
}

- (void)setDebugMode:(BOOL)enabled {
    NSAssert([EEThread isMainThread], @"");
    if (!initialized_) {
        return;
    }
    [UnityAds setDebugMode:enabled];
}

- (BOOL)hasRewardedAd:(NSString*)adId {
    NSAssert([EEThread isMainThread], @"");
    if (!initialized_) {
        return NO;
    }
    return [UnityAds isReady:adId];
}

- (void)showRewardedAd:(NSString*)adId {
    NSAssert([EEThread isMainThread], @"");
    if (!initialized_) {
        // FIXME: handle error.
        return;
    }
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [UnityAds show:rootView placementId:adId];
}

- (void)unityAdsReady:(NSString*)adId {
    NSLog(@"%s: adId = %@", __PRETTY_FUNCTION__, adId);
    [bridge_ callCpp:k__onLoaded:adId];
}

- (void)unityAdsDidError:(UnityAdsError)error withMessage:(NSString*)message {
    NSLog(@"%s: error %d message %@", __PRETTY_FUNCTION__, (int)error, message);
}

- (void)unityAdsDidStart:(NSString*)adId {
    NSLog(@"%s: adId = %@", __PRETTY_FUNCTION__, adId);
}

- (void)unityAdsDidFinish:(NSString*)adId
          withFinishState:(UnityAdsFinishState)state {
    NSLog(@"%s: adId = %@ state = %d", __PRETTY_FUNCTION__, adId, (int)state);
    if (state == kUnityAdsFinishStateError) {
        [bridge_ callCpp:k__onFailedToShow //
                        :[EEJsonUtils convertObjectToString:@{
                            @"ad_id": adId,
                            @"message": @"",
                        }]];
        return;
    }
    if (state == kUnityAdsFinishStateSkipped) {
        [bridge_ callCpp:k__onClosed //
                        :[EEJsonUtils convertObjectToString:@{
                            @"ad_id": adId,
                            @"rewarded": @(NO),
                        }]];
        return;
    }
    if (state == kUnityAdsFinishStateCompleted) {
        [bridge_ callCpp:k__onClosed //
                        :[EEJsonUtils convertObjectToString:@{
                            @"ad_id": adId,
                            @"rewarded": @(YES),
                        }]];
        return;
    }
    NSAssert(NO, @"");
}

@end
