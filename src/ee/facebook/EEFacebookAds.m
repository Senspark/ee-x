//
//  EEFacebookAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <FBAudienceNetwork/FBAudienceNetwork.h>

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebook/EEFacebookAds.h"
#import "ee/facebook/internal/EEFacebookBannerAd.h"
#import "ee/facebook/internal/EEFacebookInterstitialAd.h"
#import "ee/facebook/internal/EEFacebookNativeAd.h"

@interface EEFacebookAds () {
    NSMutableDictionary<NSString*, EEFacebookBannerAd*>* bannerAds_;
    NSMutableDictionary<NSString*, EEFacebookNativeAd*>* nativeAds_;
    NSMutableDictionary<NSString*, EEFacebookInterstitialAd*>* interstitialAds_;
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
// clang-format on

// clang-format off
static NSString* const k__ad_id                 = @"ad_id";
static NSString* const k__ad_size               = @"ad_size";
static NSString* const k__layout_name           = @"layout_name";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
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
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:k__getTestDeviceHash
                   callback:^(NSString* message) {
                       return [self getTestDeviceHash];
                   }];

    [bridge registerHandler:k__addTestDevice
                   callback:^(NSString* message) {
                       NSString* hash = message;
                       [self addTestDevice:hash];
                       return @"";
                   }];

    [bridge registerHandler:k__clearTestDevices
                   callback:^(NSString* message) {
                       [self clearTestDevices];
                       return @"";
                   }];

    [bridge registerHandler:k__createBannerAd
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

    [bridge registerHandler:k__destroyBannerAd
                   callback:^(NSString* message) {
                       NSString* adId = message;
                       return [EEUtils toString:[self destroyBannerAd:adId]];
                   }];

    [bridge
        registerHandler:k__createNativeAd
               callback:^(NSString* message) {
                   NSDictionary* dict =
                       [EEJsonUtils convertStringToDictionary:message];
                   NSString* adId = dict[k__ad_id];
                   NSString* layoutName = dict[k__layout_name];
                   return [EEUtils
                       toString:[self createNativeAd:adId layout:layoutName]];
               }];

    [bridge registerHandler:k__destroyNativeAd
                   callback:^(NSString* message) {
                       NSString* adId = message;
                       return [EEUtils toString:[self destroyNativeAd:adId]];
                   }];

    [bridge registerHandler:k__createInterstitialAd
                   callback:^(NSString* message) {
                       NSString* placementId = message;
                       return [EEUtils
                           toString:[self createInterstitialAd:placementId]];
                   }];

    [bridge registerHandler:k__destroyInterstitialAd
                   callback:^(NSString* message) {
                       NSString* placementId = message;
                       return [EEUtils
                           toString:[self destroyInterstitialAd:placementId]];
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__getTestDeviceHash];
    [bridge deregisterHandler:k__addTestDevice];
    [bridge deregisterHandler:k__clearTestDevices];

    [bridge deregisterHandler:k__createBannerAd];
    [bridge deregisterHandler:k__destroyBannerAd];

    [bridge deregisterHandler:k__createNativeAd];
    [bridge deregisterHandler:k__destroyNativeAd];

    [bridge deregisterHandler:k__createInterstitialAd];
    [bridge deregisterHandler:k__destroyInterstitialAd];
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
        [[[EEFacebookBannerAd alloc] initWithAdId:adId size:size] autorelease];
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

- (BOOL)createNativeAd:(NSString*)adId layout:(NSString*)layout {
    if ([nativeAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEFacebookNativeAd* ad =
        [[[EEFacebookNativeAd alloc] initWithAdId:adId layout:layout]
            autorelease];
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
        initWithPlacementId:placementId] autorelease];
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

@end
