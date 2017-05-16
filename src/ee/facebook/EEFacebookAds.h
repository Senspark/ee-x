//
//  EEFacebookAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

@interface EEFacebookAds : NSObject

- (void) initFBAdsInterstitial:(NSString*) InterstitialID;
- (void) initFBAdsNativeAds:(NSString*) NativeID layout:(NSString*) layout;
- (void) initFBAdsBanner:(NSString*) BannerID;


- (void)cacheRewardedAd:(NSString*)adsID;
- (void)cacheInterstitialAd:(NSString*)adsID;

- (void)showBannerAd:(NSString*)adsID pos:(int)pos;
- (void)hideBannerAd;

- (bool)hasInterstitialAd;
- (bool)hasRewardedAd;

- (void)showInterstitialAd;
- (void)showRewardedAd;

- (void)hideNativeAd:(NSString*)adsID;

- (void)showNativeAd:(NSString*)adsID
                       width:(int)width
                      height:(int)height
                           x:(int)x
                           y:(int)y;

- (void)hideNativeAdvancedAd;

- (void)onAdsCallback:(int)code msg:(NSString*)msg;

@end
