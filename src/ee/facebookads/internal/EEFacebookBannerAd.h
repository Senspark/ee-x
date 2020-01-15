//
//  EEFacebookBannerAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <ee/ads/EEIAdView.h>

// Forward.
struct FBAdSize;
typedef struct FBAdSize FBAdSize;

@protocol EEIMessageBridge;

@interface EEFacebookBannerAd : NSObject <EEIAdView>

+ (FBAdSize)adSizeFor:(int)index;

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                         adId:(NSString* _Nonnull)adId
                         size:(FBAdSize)adSize;

- (void)destroy;

@end
