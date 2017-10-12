//
//  EEInterstitialAdHelper.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <Foundation/NSObject.h>
#import <Foundation/NSString.h>

@protocol EEInterstitialAdInterface;

@interface EEInterstitialAdHelper : NSObject

- (id _Nonnull)initWithPrefix:(NSString* _Nonnull)prefix
                         adId:(NSString* _Nonnull)adId;

- (void)registerHandlers:(id<EEInterstitialAdInterface> _Nonnull)ad;
- (void)deregisterHandlers;

@end
