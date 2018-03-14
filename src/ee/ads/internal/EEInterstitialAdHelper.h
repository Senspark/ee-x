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
@class EEMessageBridge;

@interface EEInterstitialAdHelper : NSObject

- (id _Nonnull)initWithBridge:(EEMessageBridge* _Nonnull)bridge
                       prefix:(NSString* _Nonnull)prefix
                         adId:(NSString* _Nonnull)adId;

- (void)registerHandlers:(id<EEIInterstitialAd> _Nonnull)ad;
- (void)deregisterHandlers;

@end
