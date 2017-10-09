//
//  EEFacebookAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <Foundation/Foundation.h>

@interface EEFacebookAds : NSObject

- (BOOL)createBannerAd:(NSString* _Nonnull)adId size:(FBAdSize)size;
- (BOOL)destroyBannerAd:(NSString* _Nonnull)adId;

- (BOOL)createNativeAd:(NSString* _Nonnull)adId
                layout:(NSString* _Nonnull)layout;
- (BOOL)destroyNativeAd:(NSString* _Nonnull)adId;

- (void)cacheInterstitialAd:(NSString* _Nonnull)adId;
- (BOOL)hasInterstitialAd;
- (BOOL)showInterstitialAd;

@end
