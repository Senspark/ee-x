#include "ee/ads/internal/EEMessageHelper.h"

@implementation EEMessageHelper {
    NSString* prefix_;
    NSString* adId_;
}

- (id _Nonnull)initWithPrefix:(NSString* _Nonnull)prefix
                         adId:(NSString* _Nonnull)adId {
    self = [super init];
    if (self == nil) {
        return self;
    }
    [prefix_ = prefix copy];
    adId_ = [adId copy];
    return self;
}

- (void)dealloc {
    [prefix_ release];
    prefix_ = nil;
    [adId_ release];
    adId_ = nil;
    [super dealloc];
}

- (NSString* _Nonnull)isLoaded {
    return [NSString stringWithFormat:@"%@_isLoaded_%@", prefix_, adId_];
}

- (NSString* _Nonnull)load {
    return [NSString stringWithFormat:@"%@_load_%@", prefix_, adId_];
}

- (NSString* _Nonnull)onLoaded {
    return [NSString stringWithFormat:@"%@_onLoaded_%@", prefix_, adId_];
}

- (NSString* _Nonnull)onFailedToLoad {
    return [NSString stringWithFormat:@"%@_onFailedToLoad_%@", prefix_, adId_];
}

- (NSString* _Nonnull)show {
    return [NSString stringWithFormat:@"%@_show_%@", prefix_, adId_];
}

- (NSString* _Nonnull)onFailedToShow {
    return [NSString stringWithFormat:@"%@_onFailedToShow_%@", prefix_, adId_];
}

- (NSString* _Nonnull)onClicked {
    return [NSString stringWithFormat:@"%@_onClicked_%@", prefix_, adId_];
}

- (NSString* _Nonnull)onClosed {
    return [NSString stringWithFormat:@"%@_onClosed_%@", prefix_, adId_];
}

- (NSString* _Nonnull)getPosition {
    return [NSString stringWithFormat:@"%@_getPosition_%@", prefix_, adId_];
}

- (NSString* _Nonnull)setPosition {
    return [NSString stringWithFormat:@"%@_setPosition_%@", prefix_, adId_];
}

- (NSString* _Nonnull)getSize {
    return [NSString stringWithFormat:@"%@_getSize_%@", prefix_, adId_];
}

- (NSString* _Nonnull)setSize {
    return [NSString stringWithFormat:@"%@_setSize_%@", prefix_, adId_];
}

- (NSString* _Nonnull)isVisible {
    return [NSString stringWithFormat:@"%@_isVisible_%@", prefix_, adId_];
}

- (NSString* _Nonnull)setVisible {
    return [NSString stringWithFormat:@"%@_setVisible_%@", prefix_, adId_];
}

- (NSString* _Nonnull)createInternalAd {
    return
        [NSString stringWithFormat:@"%@_createInternalAd_%@", prefix_, adId_];
}

- (NSString* _Nonnull)destroyInternalAd {
    return
        [NSString stringWithFormat:@"%@_destroyInternalAd_%@", prefix_, adId_];
}

@end
