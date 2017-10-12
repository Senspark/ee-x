//
//  EEFacebookNativeAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import "ee/ads/internal/EEAdViewInterface.h"

@interface EEFacebookNativeAd : NSObject <EEAdViewInterface>

- (id _Nonnull)initWithAdId:(NSString* _Nonnull)adId
                     layout:(NSString* _Nonnull)layoutName;

@end
