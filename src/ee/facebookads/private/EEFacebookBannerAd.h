//
//  EEFacebookBannerAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <ee/ads/internal/EEIAdView.h>

// Forward.
struct FBAdSize;
typedef struct FBAdSize FBAdSize;

@protocol EEIMessageBridge;

@interface EEFacebookBannerAd : NSObject <EEIAdView>

+ (FBAdSize)adSizeFor:(int)index;

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                         adId:(NSString* _Nonnull)adId
                         size:(FBAdSize)adSize;

- (void)destroy;

@end
