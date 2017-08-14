//
//  EEFacebookAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <Foundation/Foundation.h>

@interface EEFacebookAds : NSObject

- (void)initFBAdsInterstitial:(NSString*)interstitialAdId;
- (void)initFBAdsNativeAds:(NSString*)nativeAdId layout:(NSString*)layout;
- (void)initFBAdsBanner:(NSString*)bannerAdId;

- (void)cacheRewardedAd:(NSString*)adId;
- (void)cacheInterstitialAd:(NSString*)adId;

- (void)showBannerAd:(NSString*)adsId pos:(int)pos;
- (void)hideBannerAd;

- (bool)hasInterstitialAd;
- (bool)hasRewardedAd;

- (void)showInterstitialAd;
- (void)showRewardedAd;

- (void)hideNativeAd:(NSString*)adId;

- (void)showNativeAd:(NSString*)adId
               width:(int)width
              height:(int)height
                   x:(int)x
                   y:(int)y;

- (void)onAdsCallback:(int)code msg:(NSString*)msg;

@end
