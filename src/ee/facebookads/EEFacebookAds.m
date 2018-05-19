//
//  EEFacebookAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebookads/EEFacebookAds.h"
#import "ee/facebookads/internal/EEFacebookBannerAd.h"
#import "ee/facebookads/internal/EEFacebookInterstitialAd.h"
#import "ee/facebookads/internal/EEFacebookNativeAd.h"
#import "ee/facebookads/internal/EEFacebookRewardVideoAd.h"

#ifdef EE_X_USE_IRON_SOURCE_MEDIATION
#import <ISFacebookAdapter/FBAdSize.h>
#import <ISFacebookAdapter/FBAdSettings.h>
#else // EE_X_USE_IRON_SOURCE_MEDIATION
#import <FBAudienceNetwork/FBAdSize.h>
#import <FBAudienceNetwork/FBAdSettings.h>
#endif // EE_X_USE_IRON_SOURCE_MEDIATION

@interface EEFacebookAds () {
    id<EEIMessageBridge> bridge_;
    NSMutableDictionary<NSString*, EEFacebookBannerAd*>* bannerAds_;
    NSMutableDictionary<NSString*, EEFacebookNativeAd*>* nativeAds_;
    NSMutableDictionary<NSString*, EEFacebookInterstitialAd*>* interstitialAds_;
    NSMutableDictionary<NSString*, EEFacebookRewardVideoAd*>* rewardVideoAds_;
}

@end

@implementation EEFacebookAds

// clang-format off
static NSString* const k__getTestDeviceHash     = @"FacebookAds_getTestDeviceHash";
static NSString* const k__addTestDevice         = @"FacebookAds_addTestDevice";
static NSString* const k__clearTestDevices      = @"FacebookAds_clearTestDevices";

static NSString* const k__createBannerAd        = @"FacebookAds_createBannerAd";
static NSString* const k__destroyBannerAd       = @"FacebookAds_destroyBannerAd";

static NSString* const k__createNativeAd        = @"FacebookAds_createNativeAd";
static NSString* const k__destroyNativeAd       = @"FacebookAds_destroyNativeAd";

static NSString* const k__createInterstitialAd  = @"FacebookAds_createInterstitialAd";
static NSString* const k__destroyInterstitialAd = @"FacebookAds_destroyInterstitialAd";

static NSString* const k__createRewardVideoAd   = @"FacebookAds_createRewardVideoAd";
static NSString* const k__destroyRewardVideoAd  = @"FacebookAds_destroyRewardVideoAd";
// clang-format on

// clang-format off
static NSString* const k__ad_id                 = @"ad_id";
static NSString* const k__ad_size               = @"ad_size";
static NSString* const k__layout_name           = @"layout_name";
static NSString* const k__identifiers           = @"identifiers";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = [EEMessageBridge getInstance];
    bannerAds_ = [[NSMutableDictionary alloc] init];
    nativeAds_ = [[NSMutableDictionary alloc] init];
    interstitialAds_ = [[NSMutableDictionary alloc] init];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [bannerAds_ release];
    bannerAds_ = nil;
    [nativeAds_ release];
    nativeAds_ = nil;
    [interstitialAds_ release];
    interstitialAds_ = nil;
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:k__getTestDeviceHash
                    callback:^(NSString* message) {
                        return [self getTestDeviceHash];
                    }];

    [bridge_ registerHandler:k__addTestDevice
                    callback:^(NSString* message) {
                        NSString* hash = message;
                        [self addTestDevice:hash];
                        return @"";
                    }];

    [bridge_ registerHandler:k__clearTestDevices
                    callback:^(NSString* message) {
                        [self clearTestDevices];
                        return @"";
                    }];

    [bridge_ registerHandler:k__createBannerAd
                    callback:^(NSString* message) {
                        NSDictionary* dict =
                            [EEJsonUtils convertStringToDictionary:message];
                        NSString* adId = dict[k__ad_id];
                        int adSizeIndex = [dict[k__ad_size] intValue];
                        FBAdSize adSize =
                            [EEFacebookBannerAd adSizeFor:adSizeIndex];
                        return [EEUtils
                            toString:[self createBannerAd:adId size:adSize]];
                    }];

    [bridge_ registerHandler:k__destroyBannerAd
                    callback:^(NSString* message) {
                        NSString* adId = message;
                        return [EEUtils toString:[self destroyBannerAd:adId]];
                    }];

    [bridge_
        registerHandler:k__createNativeAd
               callback:^(NSString* message) {
                   NSDictionary* dict =
                       [EEJsonUtils convertStringToDictionary:message];
                   NSString* adId = dict[k__ad_id];
                   NSString* layoutName = dict[k__layout_name];
                   NSDictionary* identifiers = dict[k__identifiers];
                   return [EEUtils
                       toString:[self createNativeAd:adId layout:layoutName identifiers: identifiers]];
               }];

    [bridge_ registerHandler:k__destroyNativeAd
                   callback:^(NSString* message) {
                       NSString* adId = message;
                       return [EEUtils toString:[self destroyNativeAd:adId]];
                   }];

    [bridge_ registerHandler:k__createInterstitialAd
                   callback:^(NSString* message) {
                       NSString* placementId = message;
                       return [EEUtils
                           toString:[self createInterstitialAd:placementId]];
                   }];

    [bridge_ registerHandler:k__destroyInterstitialAd
                   callback:^(NSString* message) {
                       NSString* placementId = message;
                       return [EEUtils
                           toString:[self destroyInterstitialAd:placementId]];
                   }];
    
    [bridge_ registerHandler:k__createRewardVideoAd
                   callback:^(NSString* message) {
                       NSString* placementId = message;
                       return [EEUtils
                               toString:[self createRewardVideoAd:placementId]];
                   }];
    
    [bridge_ registerHandler:k__destroyRewardVideoAd
                   callback:^(NSString* message) {
                       NSString* placementId = message;
                       return [EEUtils
                               toString:[self destroyRewardVideoAd:placementId]];
                   }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__getTestDeviceHash];
    [bridge_ deregisterHandler:k__addTestDevice];
    [bridge_ deregisterHandler:k__clearTestDevices];

    [bridge_ deregisterHandler:k__createBannerAd];
    [bridge_ deregisterHandler:k__destroyBannerAd];

    [bridge_ deregisterHandler:k__createNativeAd];
    [bridge_ deregisterHandler:k__destroyNativeAd];

    [bridge_ deregisterHandler:k__createInterstitialAd];
    [bridge_ deregisterHandler:k__destroyInterstitialAd];
    
    [bridge_ deregisterHandler:k__createRewardVideoAd];
    [bridge_ deregisterHandler:k__destroyRewardVideoAd];
}

- (NSString* _Nonnull)getTestDeviceHash {
    return [FBAdSettings testDeviceHash];
}

- (void)addTestDevice:(NSString* _Nonnull)hash {
    [FBAdSettings addTestDevice:hash];
}

- (void)clearTestDevices {
    [FBAdSettings clearTestDevices];
}

- (BOOL)createBannerAd:(NSString* _Nonnull)adId size:(FBAdSize)size {
    if ([bannerAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEFacebookBannerAd* ad =
        [[[EEFacebookBannerAd alloc] initWithBridge:bridge_ adId:adId size:size]
            autorelease];
    [bannerAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyBannerAd:(NSString* _Nonnull)adId {
    if ([bannerAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEFacebookBannerAd* ad = [bannerAds_ objectForKey:adId];
    [ad destroy];
    [bannerAds_ removeObjectForKey:adId];
    return YES;
}

- (BOOL)createNativeAd:(NSString*)adId
                layout:(NSString*)layout
           identifiers:(NSDictionary* _Nonnull)identifiers {
    if ([nativeAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEFacebookNativeAd* ad =
        [[[EEFacebookNativeAd alloc] initWithBridge:bridge_
                                               adId:adId
                                             layout:layout
                                        identifiers:identifiers] autorelease];
    [nativeAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyNativeAd:(NSString*)adId {
    if ([nativeAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEFacebookNativeAd* ad = [nativeAds_ objectForKey:adId];
    [ad destroy];
    [nativeAds_ removeObjectForKey:adId];
    return YES;
}

- (BOOL)createInterstitialAd:(NSString* _Nonnull)placementId {
    if ([interstitialAds_ objectForKey:placementId] != nil) {
        return NO;
    }
    EEFacebookInterstitialAd* ad = [[[EEFacebookInterstitialAd alloc]
        initWithBridge:bridge_
           placementId:placementId] autorelease];
    [interstitialAds_ setObject:ad forKey:placementId];
    return YES;
}

- (BOOL)destroyInterstitialAd:(NSString* _Nonnull)placementId {
    if ([interstitialAds_ objectForKey:placementId] == nil) {
        return NO;
    }
    EEFacebookInterstitialAd* ad = [interstitialAds_ objectForKey:placementId];
    [ad destroy];
    [interstitialAds_ removeObjectForKey:placementId];
    return YES;
}

- (BOOL)createRewardVideoAd:(NSString* _Nonnull)placementId {
    if ([rewardVideoAds_ objectForKey:placementId] != nil) {
        return NO;
    }
    EEFacebookRewardVideoAd* ad = [[[EEFacebookRewardVideoAd alloc]
        initWithBridge:bridge_
           placementId:placementId] autorelease];
    [rewardVideoAds_ setObject:ad forKey:placementId];
    return YES;
}

- (BOOL)destroyRewardVideoAd:(NSString* _Nonnull)placementId {
    if ([rewardVideoAds_ objectForKey:placementId] == nil) {
        return NO;
    }
    EEFacebookRewardVideoAd* ad = [rewardVideoAds_ objectForKey:placementId];
    [ad destroy];
    [rewardVideoAds_ removeObjectForKey:placementId];
    return YES;
}
@end
