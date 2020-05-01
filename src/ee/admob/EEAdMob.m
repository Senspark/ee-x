//
//  EEAdMob.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import "ee/admob/EEAdMob.h"

#import <ee/core/internal/EEJsonUtils.h>
#import <ee/core/internal/EEMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

#import "ee/admob/private/EEAdMobBannerAd.h"
#import "ee/admob/private/EEAdMobBannerHelper.h"
#import "ee/admob/private/EEAdMobInterstitialAd.h"
#import "ee/admob/private/EEAdMobNativeAd.h"
#import "ee/admob/private/EEAdMobRewardedAd.h"

#define kPrefix @"AdMob"

// clang-format off
static NSString* const k__initialize                = kPrefix "_initialize";
static NSString* const k__getEmulatorTestDeviceHash = kPrefix "_getEmulatorTestDeviceHash";
static NSString* const k__addTestDevice             = kPrefix "_addTestDevice";

static NSString* const k__getBannerAdSize           = kPrefix "_getBannerAdSize";
static NSString* const k__createBannerAd            = kPrefix "_createBannerAd";
static NSString* const k__destroyBannerAd           = kPrefix "_destroyBannerAd";

static NSString* const k__createNativeAd            = kPrefix "_createNativeAd";
static NSString* const k__destroyNativeAd           = kPrefix "_destroyNativeAd";

static NSString* const k__createInterstitialAd      = kPrefix "_createInterstitialAd";
static NSString* const k__destroyInterstitialAd     = kPrefix "_destroyInterstitialAd";

static NSString* const k__createRewardedAd          = kPrefix "_createRewardedAd";
static NSString* const k__destroyRewardedAd         = kPrefix "_destroyRewardedAd";

static NSString* const k__ad_id                 = @"ad_id";
static NSString* const k__ad_size               = @"ad_size";
static NSString* const k__layout_name           = @"layout_name";
// clang-format on

#undef kPrefix

@implementation EEAdMob {
    id<EEIMessageBridge> bridge_;
    EEAdMobBannerHelper* bannerHelper_;
    NSMutableArray<NSString*>* testDevices_;
    NSMutableDictionary<NSString*, EEAdMobBannerAd*>* bannerAds_;
    NSMutableDictionary<NSString*, EEAdMobNativeAd*>* nativeAds_;
    NSMutableDictionary<NSString*, EEAdMobInterstitialAd*>* interstitialAds_;
    NSMutableDictionary<NSString*, EEAdMobRewardedAd*>* rewardedAds_;
}

+ (CGSize)sizeFor:(GADAdSize)size {
    return CGSizeFromGADAdSize(size);
}

- (id)init {
    NSAssert([EEUtils isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = [EEMessageBridge getInstance];
    bannerHelper_ = [[EEAdMobBannerHelper alloc] init];
    testDevices_ = [[NSMutableArray alloc] init];
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
    [testDevices_ release];
    testDevices_ = nil;
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
    [bridge_ registerHandler:k__initialize
                    callback:^(NSString* message) {
                        NSString* applicationId = message;
                        [self initialize:applicationId];
                        return @"";
                    }];

    [bridge_ registerHandler:k__getEmulatorTestDeviceHash
                    callback:^(NSString* message) {
                        return [self getEmulatorTestDeviceHash];
                    }];

    [bridge_ registerHandler:k__addTestDevice
                    callback:^(NSString* message) {
                        NSString* hash = message;
                        [self addTestDevice:hash];
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
                        GADAdSize size = [bannerHelper_ getAdSize:sizeId];
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
                   return [EEUtils
                       toString:
                           [self createNativeAd:adId
                                           type:kGADAdLoaderAdTypeUnifiedNative
                                         layout:layoutName]];
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
    [bridge_ deregisterHandler:k__initialize];
    [bridge_ deregisterHandler:k__getEmulatorTestDeviceHash];
    [bridge_ deregisterHandler:k__addTestDevice];
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

- (void)initialize:(NSString* _Nonnull)applicationId {
    NSAssert([EEUtils isMainThread], @"");
    [[GADMobileAds sharedInstance] startWithCompletionHandler:nil];
}

- (NSString* _Nonnull)getEmulatorTestDeviceHash {
    return kGADSimulatorID;
}

- (void)addTestDevice:(NSString* _Nonnull)hash {
    NSAssert([EEUtils isMainThread], @"");
    [testDevices_ addObject:hash];
    GADMobileAds.sharedInstance.requestConfiguration.testDeviceIdentifiers =
        testDevices_;
}

- (CGSize)getBannerAdSize:(int)sizeId {
    return [bannerHelper_ getSize:sizeId];
}

- (BOOL)createBannerAd:(NSString* _Nonnull)adId size:(GADAdSize)size {
    NSAssert([EEUtils isMainThread], @"");
    if ([bannerAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEAdMobBannerAd* ad =
        [[[EEAdMobBannerAd alloc] initWithBridge:bridge_ adId:adId size:size]
            autorelease];
    [bannerAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyBannerAd:(NSString* _Nonnull)adId {
    NSAssert([EEUtils isMainThread], @"");
    if ([bannerAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEAdMobBannerAd* ad = [bannerAds_ objectForKey:adId];
    [ad destroy];
    [bannerAds_ removeObjectForKey:adId];
    return YES;
}

- (BOOL)createNativeAd:(NSString* _Nonnull)adId
                  type:(GADAdLoaderAdType _Nonnull)type
                layout:(NSString* _Nonnull)layoutName {
    NSAssert([EEUtils isMainThread], @"");
    if ([nativeAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEAdMobNativeAd* ad =
        [[[EEAdMobNativeAd alloc] initWithBridge:bridge_
                                            adId:adId
                                           types:@[type]
                                          layout:layoutName] autorelease];
    [nativeAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyNativeAd:(NSString* _Nonnull)adId {
    NSAssert([EEUtils isMainThread], @"");
    if ([nativeAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEAdMobNativeAd* ad = [nativeAds_ objectForKey:adId];
    [ad destroy];
    [nativeAds_ removeObjectForKey:adId];
    return YES;
}

- (BOOL)createInterstitialAd:(NSString* _Nonnull)adId {
    NSAssert([EEUtils isMainThread], @"");
    if ([interstitialAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEAdMobInterstitialAd* ad =
        [[[EEAdMobInterstitialAd alloc] initWithBridge:bridge_ adId:adId]
            autorelease];
    [interstitialAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyInterstitialAd:(NSString* _Nonnull)adId {
    NSAssert([EEUtils isMainThread], @"");
    if ([interstitialAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEAdMobInterstitialAd* ad = [interstitialAds_ objectForKey:adId];
    [ad destroy];
    [interstitialAds_ removeObjectForKey:adId];
    return YES;
}

- (BOOL)createRewardedAd:(NSString* _Nonnull)adId {
    NSAssert([EEUtils isMainThread], @"");
    if ([interstitialAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEAdMobRewardedAd* ad =
        [[[EEAdMobRewardedAd alloc] initWithBridge:bridge_ adId:adId]
            autorelease];
    [rewardedAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyRewardedAd:(NSString* _Nonnull)adId {
    NSAssert([EEUtils isMainThread], @"");
    if ([rewardedAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEAdMobRewardedAd* ad = [rewardedAds_ objectForKey:adId];
    [ad destroy];
    [rewardedAds_ removeObjectForKey:adId];
    return YES;
}

@end
