//
//  EEFacebookNativeAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import "ee/ads/internal/EEIAdView.h"

@class EEMessageBridge;

@interface EEFacebookNativeAd : NSObject <EEIAdView>

- (id _Nonnull)initWithBridge:(EEMessageBridge* _Nonnull)bridge
                         adId:(NSString* _Nonnull)adId
                       layout:(NSString* _Nonnull)layoutName;

- (void)destroy;

@end
