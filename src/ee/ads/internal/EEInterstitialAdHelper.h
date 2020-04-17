//
//  EEInterstitialAdHelper.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <Foundation/NSObject.h>
#import <Foundation/NSString.h>

@protocol EEIInterstitialAd;
@protocol EEIMessageBridge;

@class EEMessageHelper;

@interface EEInterstitialAdHelper : NSObject

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                           ad:(id<EEIInterstitialAd> _Nonnull)ad
                       helper:(EEMessageHelper* _Nonnull)helper;

- (void)registerHandlers;
- (void)deregisterHandlers;

@end
