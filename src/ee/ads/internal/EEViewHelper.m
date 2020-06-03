#import "ee/ads/internal/EEViewHelper.h"

#if TARGET_OS_IOS
#import <UIKit/UIView.h>
#define EE_VIEW UIView
#else // TARGET_OS_IOS
#import <AppKit/NSView.h>
#define EE_VIEW NSView
#endif // TARGET_OS_IOS

#import <ee_x-Swift.h>

@implementation EEViewHelper {
    EE_VIEW* view_;
}

- (id _Nonnull)initWithView:(EE_VIEW* _Nonnull)view {
    NSAssert([EEUtils isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return self;
    }
    view_ = view;
    return self;
}

- (CGPoint)getPosition {
    NSAssert([EEUtils isMainThread], @"");
    CGPoint position = [view_ frame].origin;
    return CGPointMake([EEUtils convertDpToPixels:position.x],
                       [EEUtils convertDpToPixels:position.y]);
}

- (void)setPosition:(CGPoint)position {
    NSAssert([EEUtils isMainThread], @"");
    CGRect frame = [view_ frame];
    frame.origin = CGPointMake([EEUtils convertPixelsToDp:position.x],
                               [EEUtils convertPixelsToDp:position.y]);
    [view_ setFrame:frame];
}

- (CGSize)getSize {
    NSAssert([EEUtils isMainThread], @"");
    CGSize size = [view_ frame].size;
    return CGSizeMake([EEUtils convertDpToPixels:size.width],
                      [EEUtils convertDpToPixels:size.height]);
}

- (void)setSize:(CGSize)size {
    NSAssert([EEUtils isMainThread], @"");
    CGRect frame = [view_ frame];
    frame.size = CGSizeMake([EEUtils convertPixelsToDp:size.width],
                            [EEUtils convertPixelsToDp:size.height]);
    [view_ setFrame:frame];
}

- (BOOL)isVisible {
    NSAssert([EEUtils isMainThread], @"");
    return [view_ isHidden];
}

- (void)setVisible:(BOOL)visible {
    NSAssert([EEUtils isMainThread], @"");
    [view_ setHidden:!visible];
}

@end

#undef EE_AD_VIEW
