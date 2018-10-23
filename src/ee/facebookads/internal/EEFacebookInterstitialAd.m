//
//  EEFacebookInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import "ee/ads/internal/EEInterstitialAdHelper.h"
#import "ee/core/EEIMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebookads/internal/EEFacebookInterstitialAd.h"

#ifdef EE_X_USE_IRON_SOURCE_MEDIATION
#import <ISFacebookAdapter/FBInterstitialAd.h>
#else // EE_X_USE_IRON_SOURCE_MEDIATION
#import <FBAudienceNetwork/FBInterstitialAd.h>
#endif // EE_X_USE_IRON_SOURCE_MEDIATION

@interface EEFacebookInterstitialAd () <FBInterstitialAdDelegate> {
    id<EEIMessageBridge> bridge_;
    NSString* placementId_;
    FBInterstitialAd* interstitialAd_;
    EEInterstitialAdHelper* helper_;
}

@end

@implementation EEFacebookInterstitialAd

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                  placementId:(NSString* _Nonnull)placementId {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    placementId_ = [placementId copy];
    interstitialAd_ = nil;
    helper_ =
        [[EEInterstitialAdHelper alloc] initWithBridge:bridge_
                                                prefix:@"FacebookInterstitialAd"
                                                  adId:placementId];
    [self createInternalAd];
    [self registerHandlers];
    return self;
}

- (void)destroy {
    [self deregisterHandlers];
    [self destroyInternalAd];
}

- (void)dealloc {
    [placementId_ release];
    placementId_ = nil;
    [super dealloc];
}

- (NSString*)k__createInternalAd {
    return [@"FacebookInterstitialAd_createInternalAd_"
        stringByAppendingString:placementId_];
}

- (NSString*)k__destroyInternalAd {
    return [@"FacebookInterstitialAd_destroyInternalAd_"
        stringByAppendingString:placementId_];
}

- (NSString*)k__onLoaded {
    return [@"FacebookInterstitialAd_onLoaded_"
        stringByAppendingString:placementId_];
}

- (NSString*)k__onFailedToLoad {
    return [@"FacebookInterstitialAd_onFailedToLoad_"
        stringByAppendingString:placementId_];
}

- (NSString*)k__onClosed {
    return [@"FacebookInterstitialAd_onClosed_"
        stringByAppendingString:placementId_];
}

- (NSString*)k__onClicked {
    return [@"FacebookInterstitialAd_onClicked_"
            stringByAppendingString:placementId_];
}

- (void)registerHandlers {
    [helper_ registerHandlers:self];
    [bridge_ registerHandler:[self k__createInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self createInternalAd]];
                    }];

    [bridge_ registerHandler:[self k__destroyInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self destroyInternalAd]];
                    }];
}

- (void)deregisterHandlers {
    [helper_ deregisterHandlers];
    [bridge_ deregisterHandler:[self k__createInternalAd]];
    [bridge_ deregisterHandler:[self k__destroyInternalAd]];
}

- (BOOL)createInternalAd {
    if (interstitialAd_ != nil) {
        return NO;
    }
    FBInterstitialAd* interstitialAd = [[[FBInterstitialAd alloc]
        initWithPlacementID:placementId_] autorelease];
    [interstitialAd setDelegate:self];
    interstitialAd_ = [interstitialAd retain];
    return YES;
}

- (BOOL)destroyInternalAd {
    if (interstitialAd_ == nil) {
        return NO;
    }
    [interstitialAd_ setDelegate:nil];
    [interstitialAd_ release];
    interstitialAd_ = nil;
    return YES;
}

- (BOOL)isLoaded {
    if (interstitialAd_ == nil) {
        return NO;
    }
    return [interstitialAd_ isAdValid];
}

- (void)load {
    if (interstitialAd_ == nil) {
        return;
    }
    [interstitialAd_ loadAd];
}

- (BOOL)show {
    if (interstitialAd_ == nil) {
        return NO;
    }
    if (![interstitialAd_ isAdValid]) {
        return NO;
    }

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    return [interstitialAd_ showAdFromRootViewController:rootView];
}

- (void)interstitialAdDidClick:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd == interstitialAd_, @"");
    [bridge_ callCpp:[self k__onClicked]];
}

- (void)interstitialAdDidClose:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd == interstitialAd_, @"");
    [bridge_ callCpp:[self k__onClosed]];
}

- (void)interstitialAdWillClose:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd == interstitialAd_, @"");
}

- (void)interstitialAdDidLoad:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd == interstitialAd_, @"");
    [bridge_ callCpp:[self k__onLoaded]];
}

- (void)interstitialAd:(FBInterstitialAd*)interstitialAd
      didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(interstitialAd == interstitialAd_, @"");
    [bridge_ callCpp:[self k__onFailedToLoad] message:[error description]];
}

- (void)interstitialAdWillLogImpression:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd == interstitialAd_, @"");
}

@end
