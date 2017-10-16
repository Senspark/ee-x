//
//  EEAdMobNativeAd.h
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#import "ee/ads/internal/EEAdViewInterface.h"

@interface EEAdMobNativeAd : NSObject <EEAdViewInterface>

- (id _Nullable)initWithId:(NSString* _Nonnull)adId
                     types:(NSArray<GADAdLoaderAdType>* _Nonnull)adTypes
                    layout:(NSString* _Nonnull)layoutName;

@end
