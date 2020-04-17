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

@class EEMessageHelper;

@interface EEAdViewHelper : NSObject

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                         view:(id<EEIAdView> _Nonnull)view
                       helper:(EEMessageHelper* _Nonnull)helper;

- (void)registerHandlers;
- (void)deregisterHandlers;

@end
