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

+ (CGPoint)getPosition:(UIView* _Nonnull)view;
+ (void)setPosition:(CGPoint)position for:(UIView* _Nonnull)view;
+ (CGSize)getSize:(UIView* _Nonnull)view;
+ (void)setSize:(CGSize)size for:(UIView* _Nonnull)view;
+ (void)setVisible:(BOOL)visible for:(UIView* _Nonnull)view;

@end
