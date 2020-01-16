//
//  EEIInterstitialAd.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <Foundation/NSObject.h>

@protocol EEIInterstitialAd <NSObject>

- (BOOL)isLoaded;
- (void)load;
- (BOOL)show;

@end
