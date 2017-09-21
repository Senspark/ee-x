//
//  EEALovinAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface EEALovinAds : NSObject

- (void) initALovinAds;

- (BOOL) isInterstitialReady;

- (void) showInterstitial;

- (BOOL) isRewardVideoReady;

- (void) showRewardVideo;


@property(assign, nonatomic) UIViewController* rootController;

@end
