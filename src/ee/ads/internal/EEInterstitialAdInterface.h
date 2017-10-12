//
//  EEInterstitialAdInterface.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <Foundation/NSObject.h>

@protocol EEInterstitialAdInterface <NSObject>

- (BOOL)isLoaded;
- (void)load;
- (void)show;

@end
