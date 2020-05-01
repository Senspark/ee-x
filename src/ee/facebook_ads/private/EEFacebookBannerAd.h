//
//  EEFacebookBannerAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <FBAudienceNetwork/FBAdSize.h>

#import <ee/ads/internal/EEIAdView.h>

@protocol EEIMessageBridge;

@interface EEFacebookBannerAd : NSObject <EEIAdView>

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                         adId:(NSString* _Nonnull)adId
                         size:(FBAdSize)adSize;

- (void)destroy;

@end
