#import "ee/ads/internal/EEViewHelper.h"

#import <ee/core/internal/EEUtils.h>

#if TARGET_OS_IOS
#import <UIKit/UIView.h>
#define EE_VIEW UIView
#else // TARGET_OS_IOS
#import <AppKit/NSView.h>
#define EE_VIEW NSView
#endif // TARGET_OS_IOS

@interface EEViewHelper () {
    EE_VIEW* view_;
}

@end

@implementation EEViewHelper

- (id _Nonnull)initWithView:(EE_VIEW* _Nonnull)view {
    self = [super init];
    if (self == nil) {
        return self;
    }
    view_ = view;
    return self;
}

- (CGPoint)getPosition {
    CGFloat scale = [EEUtils getDensity];
    CGPoint position = [view_ frame].origin;
    return CGPointMake(position.x * scale, position.y * scale);
}

- (void)setPosition:(CGPoint)position {
    CGFloat scale = [EEUtils getDensity];
    CGRect frame = [view_ frame];
    frame.origin = CGPointMake(position.x / scale, position.y / scale);
    [view_ setFrame:frame];
}

- (CGSize)getSize {
    CGFloat scale = [EEUtils getDensity];
    CGSize size = [view_ frame].size;
    return CGSizeMake(size.width * scale, size.height * scale);
}

- (void)setSize:(CGSize)size {
    CGFloat scale = [EEUtils getDensity];
    CGRect frame = [view_ frame];
    frame.size = CGSizeMake(size.width / scale, size.height / scale);
    [view_ setFrame:frame];
}

- (BOOL)isVisible {
    return [view_ isHidden];
}

- (void)setVisible:(BOOL)visible {
    [view_ setHidden:!visible];
}

@end

#undef EE_AD_VIEW
