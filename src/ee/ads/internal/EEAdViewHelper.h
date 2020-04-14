//
//  EEAdViewUtils.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <Foundation/Foundation.h>
#import <Foundation/NSString.h>

@protocol EEIAdView;
@protocol EEIMessageBridge;

@interface EEAdViewHelper : NSObject

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                         view:(id<EEIAdView> _Nonnull)view
                       prefix:(NSString* _Nonnull)prefix
                         adId:(NSString* _Nonnull)adId;

- (void)registerHandlers;
- (void)deregisterHandlers;

@end

#undef EE_AD_VIEW
