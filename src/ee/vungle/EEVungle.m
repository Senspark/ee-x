//
//  EEVungle.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/vungle/EEVungle.h"

#import <UIKit/UIKit.h>

#import <VungleSDK/VungleSDK.h>

#import <ee/core/internal/EEJsonUtils.h>
#import <ee/core/internal/EEMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

#define kPrefix @"Vungle"

// clang-format off
static NSString* const k__initialize     = kPrefix "_initialize";

static NSString* const k__hasRewardedAd  = kPrefix "_hasRewardedAd";
static NSString* const k__loadRewardedAd = kPrefix "_loadRewardedAd";
static NSString* const k__showRewardedAd = kPrefix "_showRewardedAd";

static NSString* const k__onLoaded       = kPrefix "_onLoaded";
static NSString* const k__onFailedToLoad = kPrefix "_onFailedToLoad";
static NSString* const k__onFailedToShow = kPrefix "_onFailedToShow";
static NSString* const k__onClosed       = kPrefix "_onClosed";
// clang-format on

#undef kPrefix

@interface EEVungle () <VungleSDKDelegate>
@end

@implementation EEVungle {
    EEMessageBridge* bridge_;
    BOOL initialized_;
    VungleSDK* sdk_;
    NSSet<NSString*>* loadingAdIds_;
}

- (id)init {
    NSAssert([EEUtils isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return self;
    }
    initialized_ = NO;
    bridge_ = [EEMessageBridge getInstance];
    sdk_ = [VungleSDK sharedSDK];
    loadingAdIds_ = [[NSMutableSet alloc] init];

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)destroy {
    NSAssert([EEUtils isMainThread], @"");
    [self deregisterHandlers];
    [loadingAdIds_ release];
    loadingAdIds_ = nil;
    if (!initialized_) {
        return;
    }
    [sdk_ setDelegate:nil];
}

- (void)registerHandlers {
    [bridge_ registerHandler:k__initialize
                    callback:^(NSString* message) {
                        NSDictionary* dict =
                            [EEJsonUtils convertStringToDictionary:message];
                        NSString* gameId = dict[@"gameId"];
                        [self initialize:gameId];
                        return @"";
                    }];

    [bridge_ registerHandler:k__hasRewardedAd
                    callback:^(NSString* adId) {
                        return [EEUtils toString:[self hasRewardedAd:adId]];
                    }];

    [bridge_ registerHandler:k__loadRewardedAd
                    callback:^(NSString* adId) {
                        [self loadRewardedAd:adId];
                        return @"";
                    }];

    [bridge_ registerHandler:k__showRewardedAd
                    callback:^(NSString* adId) {
                        [self showRewardedAd:adId];
                        return @"";
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__initialize];
    [bridge_ deregisterHandler:k__hasRewardedAd];
    [bridge_ deregisterHandler:k__loadRewardedAd];
    [bridge_ deregisterHandler:k__showRewardedAd];
}

- (void)initialize:(NSString*)gameId {
    NSAssert([EEUtils isMainThread], @"");
    if (initialized_) {
        return;
    }
    NSError* error = nil;
    [sdk_ startWithAppId:gameId error:&error];
    if (error != nil) {
        NSLog(@"%s: %@", __PRETTY_FUNCTION__, error);
        return;
    }
    [sdk_ setDelegate:self];
    initialized_ = YES;
}

- (BOOL)hasRewardedAd:(NSString* _Nonnull)adId {
    NSAssert([EEUtils isMainThread], @"");
    return [sdk_ isAdCachedForPlacementID:adId];
}

- (void)loadRewardedAd:(NSString*)adId {
    NSAssert([EEUtils isMainThread], @"");
    NSError* error = nil;
    [sdk_ loadPlacementWithID:adId error:&error];
    if (error != nil) {
        [bridge_ callCpp:k__onFailedToLoad
                 message:[EEJsonUtils convertDictionaryToString:@{
                     @"ad_id": adId,
                     @"message": [error description],
                 }]];
        return;
    }
    // OK.
}

- (void)showRewardedAd:(NSString*)adId {
    NSAssert([EEUtils isMainThread], @"");
    UIViewController* view = [EEUtils getCurrentRootViewController];
    NSError* error = nil;
    [sdk_ playAd:view options:nil placementID:adId error:&error];
    if (error != nil) {
        [bridge_ callCpp:k__onFailedToShow
                 message:[EEJsonUtils convertDictionaryToString:@{
                     @"ad_id": adId,
                     @"message": [error description],
                 }]];
        return;
    }
    // OK.
}

- (void)vungleAdPlayabilityUpdate:(BOOL)isAdPlayable
                      placementID:(nullable NSString*)adId
                            error:(nullable NSError*)error {
    NSLog(@"%s: playable = %d id = %@ reason = %@", __PRETTY_FUNCTION__,
          (int)isAdPlayable, adId, error.description);
    if ([loadingAdIds_ containsObject:adId]) {
        NSAssert(isAdPlayable, @"");
        [bridge_ callCpp:k__onLoaded
                 message:[EEJsonUtils convertDictionaryToString:@{
                     @"ad_id": adId,
                 }]];
        return;
    }
    if (error != nil) {
        NSAssert(adId == nil, @"");
        return;
    }
    // Mediation ???
}

- (void)vungleWillShowAdForPlacementID:(nullable NSString*)adId {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, adId);
}

- (void)vungleDidShowAdForPlacementID:(nullable NSString*)adId {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, adId);
}

- (void)vungleDidCloseAdWithViewInfo:(VungleViewInfo*)info
                         placementID:(NSString*)adId {
    BOOL result = [[info completedView] boolValue];
    [bridge_ callCpp:k__onClosed
             message:[EEJsonUtils convertDictionaryToString:@{
                 @"ad_id": adId,
                 @"rewarded": @(result),
             }]];
}

@end
