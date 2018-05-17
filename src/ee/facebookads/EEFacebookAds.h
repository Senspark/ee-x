//
//  EEFacebookAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <Foundation/Foundation.h>

// Forward.
struct FBAdSize;
typedef struct FBAdSize FBAdSize;

@interface EEFacebookAds : NSObject

- (NSString* _Nonnull)getTestDeviceHash;
- (void)addTestDevice:(NSString* _Nonnull)hash;
- (void)clearTestDevices;

- (BOOL)createBannerAd:(NSString* _Nonnull)adId size:(FBAdSize)size;
- (BOOL)destroyBannerAd:(NSString* _Nonnull)adId;

- (BOOL)createNativeAd:(NSString* _Nonnull)adId
                layout:(NSString* _Nonnull)layout;
- (BOOL)destroyNativeAd:(NSString* _Nonnull)adId;

- (BOOL)createInterstitialAd:(NSString* _Nonnull)placementId;
- (BOOL)destroyInterstitialAd:(NSString* _Nonnull)placementId;

@end
