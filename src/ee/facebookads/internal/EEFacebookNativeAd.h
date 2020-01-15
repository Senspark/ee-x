//
//  EEFacebookNativeAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <ee/ads/EEIAdView.h>

@protocol EEIMessageBridge;

@interface EEFacebookNativeAd : NSObject <EEIAdView>

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                         adId:(NSString* _Nonnull)adId
                       layout:(NSString* _Nonnull)layoutName
                  identifiers:(NSDictionary* _Nonnull)identifiers;

- (void)destroy;

@end
