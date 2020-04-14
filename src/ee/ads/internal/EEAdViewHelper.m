//
//  EEAdViewUtils.m
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/ads/internal/EEAdViewHelper.h"

#import <ee/core/internal/EEIMessageBridge.h>
#import <ee/core/internal/EEJsonUtils.h>
#import <ee/core/internal/EEUtils.h>

#import "ee/ads/internal/EEIAdView.h"

@interface EEAdViewHelper () {
    id<EEIMessageBridge> bridge_;
    NSString* prefix_;
    NSString* adId_;
    id<EEIAdView> view_;
}

@end

@implementation EEAdViewHelper

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                         view:(id<EEIAdView> _Nonnull)view
                       prefix:(NSString* _Nonnull)prefix
                         adId:(NSString* _Nonnull)adId {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    view_ = view;
    prefix_ = [prefix copy];
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

- (NSString* _Nonnull)k__isLoaded {
    return [NSString stringWithFormat:@"%@_isLoaded_%@", prefix_, adId_];
}

- (NSString* _Nonnull)k__load {
    return [NSString stringWithFormat:@"%@_load_%@", prefix_, adId_];
}

- (NSString* _Nonnull)k__getPosition {
    return [NSString stringWithFormat:@"%@_getPosition_%@", prefix_, adId_];
}

- (NSString* _Nonnull)k__setPosition {
    return [NSString stringWithFormat:@"%@_setPosition_%@", prefix_, adId_];
}

- (NSString* _Nonnull)k__getSize {
    return [NSString stringWithFormat:@"%@_getSize_%@", prefix_, adId_];
}

- (NSString* _Nonnull)k__setSize {
    return [NSString stringWithFormat:@"%@_setSize_%@", prefix_, adId_];
}

- (NSString* _Nonnull)k__isVisible {
    return [NSString stringWithFormat:@"%@_isVisible_%@", prefix_, adId_];
}

- (NSString* _Nonnull)k__setVisible {
    return [NSString stringWithFormat:@"%@_setVisible_%@", prefix_, adId_];
}

- (void)registerHandlers {
    [bridge_ registerHandler:[self k__isLoaded]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[view_ isLoaded]];
                    }];

    [bridge_ registerHandler:[self k__load]
                    callback:^(NSString* message) {
                        [view_ load];
                        return @"";
                    }];

    [bridge_ registerHandler:[self k__getPosition]
                    callback:^(NSString* message) {
                        CGPoint position = [view_ getPosition];
                        NSMutableDictionary* dict =
                            [NSMutableDictionary dictionary];
                        [dict setValue:@(position.x) forKey:@"x"];
                        [dict setValue:@(position.y) forKey:@"y"];
                        return [EEJsonUtils convertDictionaryToString:dict];
                    }];

    [bridge_ registerHandler:[self k__setPosition]
                    callback:^(NSString* message) {
                        NSDictionary* dict =
                            [EEJsonUtils convertStringToDictionary:message];
                        int x = [dict[@"x"] intValue];
                        int y = [dict[@"y"] intValue];
                        [view_ setPosition:CGPointMake(x, y)];
                        return @"";
                    }];

    [bridge_ registerHandler:[self k__getSize]
                    callback:^(NSString* message) {
                        CGSize size = [view_ getSize];
                        NSMutableDictionary* dict =
                            [NSMutableDictionary dictionary];
                        [dict setValue:@(size.width) forKey:@"width"];
                        [dict setValue:@(size.height) forKey:@"height"];
                        return [EEJsonUtils convertDictionaryToString:dict];
                    }];

    [bridge_ registerHandler:[self k__setSize]
                    callback:^(NSString* message) {
                        NSDictionary* dict =
                            [EEJsonUtils convertStringToDictionary:message];
                        int width = [dict[@"width"] intValue];
                        int height = [dict[@"height"] intValue];
                        [view_ setSize:CGSizeMake(width, height)];
                        return @"";
                    }];

    [bridge_ registerHandler:[self k__isVisible]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[view_ isVisible]];
                    }];

    [bridge_ registerHandler:[self k__setVisible]
                    callback:^(NSString* message) {
                        [view_ setVisible:[EEUtils toBool:message]];
                        return @"";
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:[self k__isLoaded]];
    [bridge_ deregisterHandler:[self k__load]];
    [bridge_ deregisterHandler:[self k__getPosition]];
    [bridge_ deregisterHandler:[self k__setPosition]];
    [bridge_ deregisterHandler:[self k__getSize]];
    [bridge_ deregisterHandler:[self k__setSize]];
    [bridge_ deregisterHandler:[self k__isVisible]];
    [bridge_ deregisterHandler:[self k__setVisible]];
}

@end
