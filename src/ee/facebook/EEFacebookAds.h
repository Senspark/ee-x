//
//  EEFacebookAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

@interface EEFacebookAds : NSObject

- (void) initFBAds:(NSString*) InterstitialID nativeID:(NSString*) NativeID bannerID:(NSString*) BannerID;
- (void)cacheRewardedAd:(NSString*)adsID;
- (void)cacheInterstitialAd:(NSString*)adsID;

- (void)showBannerAd:(NSString*)adsID pos:(int)pos;
- (void)hideBannerAd;

- (bool)hasInterstitialAd;
- (bool)hasRewardedAd;

- (void)showInterstitialAd;
- (void)showRewardedAd;

- (void)showNativeExpressAd:(NSString*)adsID
                      width:(int)width
                     height:(int)height
                        pos:(int)pos;
- (void)showNativeExpressAdWithDeltaPosition:(NSString*)adsId
                                       width:(int)width
                                      height:(int)height
                                      deltaX:(int)deltaX
                                      deltaY:(int)deltaY;
- (void)hideNativeExpressAd;

- (void)showNativeAdvancedAd:(NSString*)adsID
                       width:(int)width
                      height:(int)height
                           x:(int)x
                           y:(int)y;

- (void)hideNativeAdvancedAd;

- (void)onAdsCallback:(int)code msg:(NSString*)msg;

@end
