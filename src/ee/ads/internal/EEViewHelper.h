#import <CoreGraphics/CGGeometry.h>
#import <TargetConditionals.h>

#if TARGET_OS_IOS
@class UIView;
#define EE_VIEW UIView
#else // TARGET_OS_IOS
@class NSView;
#define EE_VIEW NSView
#endif // TARGET_OS_IOS

@interface EEViewHelper : NSObject

- (id _Nonnull)initWithView:(EE_VIEW* _Nonnull)view;

- (CGPoint)getPosition;
- (void)setPosition:(CGPoint)position;

- (CGSize)getSize;
- (void)setSize:(CGSize)size;

- (BOOL)isVisible;
- (void)setVisible:(BOOL)visible;

@end

#undef EE_VIEW
