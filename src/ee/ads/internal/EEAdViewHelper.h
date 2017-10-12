//
//  EEAdViewUtils.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <Foundation/NSObject.h>
#import <Foundation/NSString.h>

@protocol EEAdViewInterface;

@interface EEAdViewHelper : NSObject

- (id _Nonnull)initWithPrefix:(NSString* _Nonnull)prefix
                         adId:(NSString* _Nonnull)adId;

- (void)registerHandlers:(id<EEAdViewInterface> _Nonnull)adView;
- (void)deregisterHandlers;

@end
