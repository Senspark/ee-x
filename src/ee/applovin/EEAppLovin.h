//
//  EEALovinAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <Foundation/Foundation.h>

@interface EEAppLovin : NSObject

- (void)initialize;

- (BOOL)isInterstitialAdReady;

- (BOOL)showInterstitialAd;

- (void)loadRewardedVideo;

- (BOOL)isRewardedVideoReady;

- (BOOL)showRewardedVideo;

@end
