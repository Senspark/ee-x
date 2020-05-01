//
//  EEFacebookAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/facebook_ads/EEFacebookAds.h"

#import <FBAudienceNetwork/FBAdSettings.h>

#import <ee/core/internal/EEJsonUtils.h>
#import <ee/core/internal/EEMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

#import "ee/facebook_ads/private/EEFacebookBannerAd.h"
#import "ee/facebook_ads/private/EEFacebookBannerHelper.h"
#import "ee/facebook_ads/private/EEFacebookInterstitialAd.h"
#import "ee/facebook_ads/private/EEFacebookNativeAd.h"
#import "ee/facebook_ads/private/EEFacebookRewardedAd.h"

#define kPrefix @"FacebookAds"

// clang-format off
static NSString* const k__getTestDeviceHash     = kPrefix "_getTestDeviceHash";
static NSString* const k__addTestDevice         = kPrefix "_addTestDevice";
static NSString* const k__clearTestDevices      = kPrefix "_clearTestDevices";

static NSString* const k__getBannerAdSize       = kPrefix "_getBannerAdSize";
static NSString* const k__createBannerAd        = kPrefix "_createBannerAd";
static NSString* const k__destroyBannerAd       = kPrefix "_destroyBannerAd";

static NSString* const k__createNativeAd        = kPrefix "_createNativeAd";
static NSString* const k__destroyNativeAd       = kPrefix "_destroyNativeAd";

static NSString* const k__createInterstitialAd  = kPrefix "_createInterstitialAd";
static NSString* const k__destroyInterstitialAd = kPrefix "_destroyInterstitialAd";

static NSString* const k__createRewardedAd      = kPrefix "_createRewardedAd";
static NSString* const k__destroyRewardedAd     = kPrefix "_destroyRewardedAd";
// clang-format on

// clang-format off
static NSString* const k__ad_id                 = @"ad_id";
static NSString* const k__ad_size               = @"ad_size";
static NSString* const k__layout_name           = @"layout_name";
static NSString* const k__identifiers           = @"identifiers";
// clang-format on

#undef kPrefix

@implementation EEFacebookAds {
    id<EEIMessageBridge> bridge_;
    EEFacebookBannerHelper* bannerHelper_;
    NSMutableDictionary<NSString*, EEFacebookBannerAd*>* bannerAds_;
    NSMutableDictionary<NSString*, EEFacebookNativeAd*>* nativeAds_;
    NSMutableDictionary<NSString*, EEFacebookInterstitialAd*>* interstitialAds_;
    NSMutableDictionary<NSString*, EEFacebookRewardedAd*>* rewardedAds_;
}

- (id)init {
    NSAssert([EEUtils isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = [EEMessageBridge getInstance];
    bannerHelper_ = [[EEFacebookBannerHelper alloc] init];
    bannerAds_ = [[NSMutableDictionary alloc] init];
    nativeAds_ = [[NSMutableDictionary alloc] init];
    interstitialAds_ = [[NSMutableDictionary alloc] init];
    rewardedAds_ = [[NSMutableDictionary alloc] init];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)destroy {
    NSAssert([EEUtils isMainThread], @"");
    [self deregisterHandlers];
    [bannerHelper_ release];
    bannerHelper_ = nil;
    [bannerAds_ release];
    bannerAds_ = nil;
    [nativeAds_ release];
    nativeAds_ = nil;
    [interstitialAds_ release];
    interstitialAds_ = nil;
    [rewardedAds_ release];
    rewardedAds_ = nil;
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

    [bridge_ registerHandler:k__getBannerAdSize
                    callback:^(NSString* message) {
                        CGSize size = [self getBannerAdSize:[message intValue]];
                        NSMutableDictionary* dict =
                            [NSMutableDictionary dictionary];
                        [dict setValue:@(size.width) forKey:@"width"];
                        [dict setValue:@(size.height) forKey:@"height"];
                        return [EEJsonUtils convertDictionaryToString:dict];
                    }];

    [bridge_ registerHandler:k__createBannerAd
                    callback:^(NSString* message) {
                        NSDictionary* dict =
                            [EEJsonUtils convertStringToDictionary:message];
                        NSString* adId = dict[k__ad_id];
                        int sizeId = [dict[k__ad_size] intValue];
                        FBAdSize size = [bannerHelper_ getAdSize:sizeId];
                        return [EEUtils
                            toString:[self createBannerAd:adId size:size]];
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
                   return [EEUtils toString:[self createNativeAd:adId
                                                          layout:layoutName
                                                     identifiers:identifiers]];
               }];

    [bridge_ registerHandler:k__destroyNativeAd
                    callback:^(NSString* message) {
                        NSString* adId = message;
                        return [EEUtils toString:[self destroyNativeAd:adId]];
                    }];

    [bridge_ registerHandler:k__createInterstitialAd
                    callback:^(NSString* message) {
                        NSString* adId = message;
                        return
                            [EEUtils toString:[self createInterstitialAd:adId]];
                    }];

    [bridge_
        registerHandler:k__destroyInterstitialAd
               callback:^(NSString* message) {
                   NSString* adId = message;
                   return [EEUtils toString:[self destroyInterstitialAd:adId]];
               }];

    [bridge_ registerHandler:k__createRewardedAd
                    callback:^(NSString* message) {
                        NSString* adId = message;
                        return [EEUtils toString:[self createRewardedAd:adId]];
                    }];

    [bridge_ registerHandler:k__destroyRewardedAd
                    callback:^(NSString* message) {
                        NSString* adId = message;
                        return [EEUtils toString:[self destroyRewardedAd:adId]];
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__getTestDeviceHash];
    [bridge_ deregisterHandler:k__addTestDevice];
    [bridge_ deregisterHandler:k__clearTestDevices];

    [bridge_ deregisterHandler:k__getBannerAdSize];
    [bridge_ deregisterHandler:k__createBannerAd];
    [bridge_ deregisterHandler:k__destroyBannerAd];

    [bridge_ deregisterHandler:k__createNativeAd];
    [bridge_ deregisterHandler:k__destroyNativeAd];

    [bridge_ deregisterHandler:k__createInterstitialAd];
    [bridge_ deregisterHandler:k__destroyInterstitialAd];

    [bridge_ deregisterHandler:k__createRewardedAd];
    [bridge_ deregisterHandler:k__destroyRewardedAd];
}

- (NSString* _Nonnull)getTestDeviceHash {
    return [FBAdSettings testDeviceHash];
}

- (void)addTestDevice:(NSString* _Nonnull)hash {
    NSAssert([EEUtils isMainThread], @"");
    [FBAdSettings addTestDevice:hash];
}

- (void)clearTestDevices {
    NSAssert([EEUtils isMainThread], @"");
    [FBAdSettings clearTestDevices];
}

- (CGSize)getBannerAdSize:(int)sizeId {
    return [bannerHelper_ getSize:sizeId];
}

- (BOOL)createBannerAd:(NSString* _Nonnull)adId size:(FBAdSize)size {
    NSAssert([EEUtils isMainThread], @"");
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
    NSAssert([EEUtils isMainThread], @"");
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
    NSAssert([EEUtils isMainThread], @"");
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
    NSAssert([EEUtils isMainThread], @"");
    if ([nativeAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEFacebookNativeAd* ad = [nativeAds_ objectForKey:adId];
    [ad destroy];
    [nativeAds_ removeObjectForKey:adId];
    return YES;
}

- (BOOL)createInterstitialAd:(NSString* _Nonnull)adId {
    NSAssert([EEUtils isMainThread], @"");
    if ([interstitialAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEFacebookInterstitialAd* ad =
        [[[EEFacebookInterstitialAd alloc] initWithBridge:bridge_ adId:adId]
            autorelease];
    [interstitialAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyInterstitialAd:(NSString* _Nonnull)adId {
    NSAssert([EEUtils isMainThread], @"");
    if ([interstitialAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEFacebookInterstitialAd* ad = [interstitialAds_ objectForKey:adId];
    [ad destroy];
    [interstitialAds_ removeObjectForKey:adId];
    return YES;
}

- (BOOL)createRewardedAd:(NSString* _Nonnull)adId {
    NSAssert([EEUtils isMainThread], @"");
    if ([rewardedAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEFacebookRewardedAd* ad =
        [[[EEFacebookRewardedAd alloc] initWithBridge:bridge_ adId:adId]
            autorelease];
    [rewardedAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyRewardedAd:(NSString* _Nonnull)adId {
    NSAssert([EEUtils isMainThread], @"");
    if ([rewardedAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEFacebookRewardedAd* ad = [rewardedAds_ objectForKey:adId];
    [ad destroy];
    [rewardedAds_ removeObjectForKey:adId];
    return YES;
}

@end
