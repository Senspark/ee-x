//
//  EEAdViewUtils.m
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/ads/internal/EEAdViewHelper.h"

#import <ee_x-Swift.h>

#import <ee/core/internal/EEJsonUtils.h>

#import "ee/ads/internal/EEIAdView.h"
#import "ee/ads/internal/EEMessageHelper.h"

@implementation EEAdViewHelper {
    id<EEIMessageBridge> bridge_;
    id<EEIAdView> view_;
    EEMessageHelper* helper_;
}

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                         view:(id<EEIAdView> _Nonnull)view
                       helper:(EEMessageHelper* _Nonnull)helper {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    view_ = view;
    helper_ = [helper retain];
    return self;
}

- (void)dealloc {
    [helper_ release];
    helper_ = nil;
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:[helper_ isLoaded]:^(NSString* message) {
        return [EEUtils toString:[view_ isLoaded]];
    }];

    [bridge_ registerHandler:[helper_ load]:^(NSString* message) {
        [view_ load];
        return @"";
    }];

    [bridge_ registerHandler:[helper_ getPosition]:^(NSString* message) {
        CGPoint position = [view_ getPosition];
        NSMutableDictionary* dict = [NSMutableDictionary dictionary];
        [dict setValue:@(position.x) forKey:@"x"];
        [dict setValue:@(position.y) forKey:@"y"];
        return [EEJsonUtils convertDictionaryToString:dict];
    }];

    [bridge_ registerHandler:[helper_ setPosition]:^(NSString* message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        int x = [dict[@"x"] intValue];
        int y = [dict[@"y"] intValue];
        [view_ setPosition:CGPointMake(x, y)];
        return @"";
    }];

    [bridge_ registerHandler:[helper_ getSize]:^(NSString* message) {
        CGSize size = [view_ getSize];
        NSMutableDictionary* dict = [NSMutableDictionary dictionary];
        [dict setValue:@(size.width) forKey:@"width"];
        [dict setValue:@(size.height) forKey:@"height"];
        return [EEJsonUtils convertDictionaryToString:dict];
    }];

    [bridge_ registerHandler:[helper_ setSize]:^(NSString* message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        int width = [dict[@"width"] intValue];
        int height = [dict[@"height"] intValue];
        [view_ setSize:CGSizeMake(width, height)];
        return @"";
    }];

    [bridge_ registerHandler:[helper_ isVisible]:^(NSString* message) {
        return [EEUtils toString:[view_ isVisible]];
    }];

    [bridge_ registerHandler:[helper_ setVisible]:^(NSString* message) {
        [view_ setVisible:[EEUtils toBool:message]];
        return @"";
    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:[helper_ isLoaded]];
    [bridge_ deregisterHandler:[helper_ load]];
    [bridge_ deregisterHandler:[helper_ getPosition]];
    [bridge_ deregisterHandler:[helper_ setPosition]];
    [bridge_ deregisterHandler:[helper_ getSize]];
    [bridge_ deregisterHandler:[helper_ setSize]];
    [bridge_ deregisterHandler:[helper_ isVisible]];
    [bridge_ deregisterHandler:[helper_ setVisible]];
}

@end
