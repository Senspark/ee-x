//
//  EEFacebookInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import <FBAudienceNetwork/FBAudienceNetwork.h>

#import "ee/ads/internal/EEInterstitialAdHelper.h"
#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebook/internal/EEFacebookInterstitialAd.h"

@interface EEFacebookInterstitialAd () <FBInterstitialAdDelegate> {
    NSString* placementId_;
    FBInterstitialAd* interstitialAd_;
    EEInterstitialAdHelper* helper_;
}

@end

@implementation EEFacebookInterstitialAd

- (id _Nonnull)initWithPlacementId:(NSString* _Nonnull)placementId {
    self = [super init];
    if (self == nil) {
        return self;
    }
    placementId_ = [placementId copy];
    interstitialAd_ = nil;
    helper_ =
        [[EEInterstitialAdHelper alloc] initWithPrefix:@"FacebookInterstitialAd"
                                                  adId:placementId];
    [self registerHandlers];
    return self;
}

- (void)destroy {
    [self deregisterHandlers];
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

- (void)registerHandlers {
    [helper_ registerHandlers:self];

    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:[self k__createInternalAd]
                   callback:^(NSString* message) {
                       return [EEUtils toString:[self createInternalAd]];
                   }];

    [bridge registerHandler:[self k__destroyInternalAd]
                   callback:^(NSString* message) {
                       return [EEUtils toString:[self destroyInternalAd]];
                   }];
}

- (void)deregisterHandlers {
    [helper_ deregisterHandlers];

    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:[self k__createInternalAd]];
    [bridge deregisterHandler:[self k__destroyInternalAd]];
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
}

- (void)interstitialAdDidClose:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd == interstitialAd_, @"");
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onClosed]];
}

- (void)interstitialAdWillClose:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd == interstitialAd_, @"");
}

- (void)interstitialAdDidLoad:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd == interstitialAd_, @"");
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onLoaded]];
}

- (void)interstitialAd:(FBInterstitialAd*)interstitialAd
      didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(interstitialAd == interstitialAd_, @"");
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onFailedToLoad] message:[error description]];
}

- (void)interstitialAdWillLogImpression:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd == interstitialAd_, @"");
}

@end
