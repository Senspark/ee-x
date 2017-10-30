//
//  EEAdViewUtils.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <CoreGraphics/CGGeometry.h>
#import <Foundation/Foundation.h>
#import <Foundation/NSString.h>
#import <TargetConditionals.h>

#if TARGET_OS_IOS
@class UIView;
#define EE_AD_VIEW UIView
#else // TARGET_OS_IOS
@class NSView;
#define EE_AD_VIEW NSView
#endif // TARGET_OS_IOS

@protocol EEAdViewInterface;

@interface EEAdViewHelper : NSObject

- (id _Nonnull)initWithPrefix:(NSString* _Nonnull)prefix
                         adId:(NSString* _Nonnull)adId;

- (void)registerHandlers:(id<EEAdViewInterface> _Nonnull)adView;
- (void)deregisterHandlers;

+ (CGPoint)getPosition:(EE_AD_VIEW* _Nonnull)view;
+ (void)setPosition:(CGPoint)position for:(EE_AD_VIEW* _Nonnull)view;
+ (CGSize)getSize:(EE_AD_VIEW* _Nonnull)view;
+ (void)setSize:(CGSize)size for:(EE_AD_VIEW* _Nonnull)view;
+ (void)setVisible:(BOOL)visible for:(EE_AD_VIEW* _Nonnull)view;

@end

#undef EE_AD_VIEW
