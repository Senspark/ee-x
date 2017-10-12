//
//  EEAdViewUtils.m
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/ads/internal/EEAdViewHelper.h"
#import "ee/ads/internal/EEAdViewInterface.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"

@interface EEAdViewHelper () {
    NSString* prefix_;
    NSString* adId_;
}

@end

@implementation EEAdViewHelper

- (id _Nonnull)initWithPrefix:(NSString* _Nonnull)prefix
                         adId:(NSString* _Nonnull)adId {
    self = [super init];
    if (self == nil) {
        return self;
    }
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

- (NSString*)k__isLoaded {
    return [NSString stringWithFormat:@"%@_isLoaded_%@", prefix_, adId_];
}

- (NSString*)k__load {
    return [NSString stringWithFormat:@"%@_load_%@", prefix_, adId_];
}

- (NSString*)k__getPosition {
    return [NSString stringWithFormat:@"%@_getPosition_%@", prefix_, adId_];
}

- (NSString*)k__setPosition {
    return [NSString stringWithFormat:@"%@_setPosition_%@", prefix_, adId_];
}

- (NSString*)k__getSize {
    return [NSString stringWithFormat:@"%@_getSize_%@", prefix_, adId_];
}

- (NSString*)k__setSize {
    return [NSString stringWithFormat:@"%@_setSize_%@", prefix_, adId_];
}

- (NSString*)k__setVisible {
    return [NSString stringWithFormat:@"%@_setVisible_%@", prefix_, adId_];
}

- (void)registerHandlers:(id<EEAdViewInterface> _Nonnull)adView {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:[self k__isLoaded]
                   callback:^(NSString* message) {
                       return [adView isLoaded] ? @"true" : @"false";
                   }];

    [bridge registerHandler:[self k__load]
                   callback:^(NSString* message) {
                       [adView load];
                       return @"";
                   }];

    [bridge registerHandler:[self k__getPosition]
                   callback:^(NSString* message) {
                       CGPoint position = [adView getPosition];
                       NSMutableDictionary* dict =
                           [NSMutableDictionary dictionary];
                       [dict setValue:@(position.x) forKey:@"x"];
                       [dict setValue:@(position.y) forKey:@"y"];
                       return [EEJsonUtils convertDictionaryToString:dict];
                   }];

    [bridge registerHandler:[self k__setPosition]
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       int x = [dict[@"x"] intValue];
                       int y = [dict[@"y"] intValue];
                       [adView setPosition:CGPointMake(x, y)];
                       return @"";
                   }];

    [bridge registerHandler:[self k__getSize]
                   callback:^(NSString* message) {
                       CGSize size = [adView getSize];
                       NSMutableDictionary* dict =
                           [NSMutableDictionary dictionary];
                       [dict setValue:@(size.width) forKey:@"width"];
                       [dict setValue:@(size.height) forKey:@"height"];
                       return [EEJsonUtils convertDictionaryToString:dict];
                   }];

    [bridge registerHandler:[self k__setSize]
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       int width = [dict[@"width"] intValue];
                       int height = [dict[@"height"] intValue];
                       [adView setSize:CGSizeMake(width, height)];
                       return @"";
                   }];

    [bridge registerHandler:[self k__setVisible]
                   callback:^(NSString* message) {
                       [adView setVisible:[message isEqualToString:@"true"]];
                       return @"";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:[self k__isLoaded]];
    [bridge deregisterHandler:[self k__load]];
    [bridge deregisterHandler:[self k__getPosition]];
    [bridge deregisterHandler:[self k__setPosition]];
    [bridge deregisterHandler:[self k__getSize]];
    [bridge deregisterHandler:[self k__setSize]];
    [bridge deregisterHandler:[self k__setVisible]];
}

@end
