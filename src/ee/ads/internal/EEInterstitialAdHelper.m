//
//  InterstitialAdInterface.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/ads/internal/EEInterstitialAdHelper.h"
#import "ee/ads/internal/EEInterstitialAdInterface.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"

@interface EEInterstitialAdHelper () {
    NSString* prefix_;
    NSString* adId_;
}

@end

@implementation EEInterstitialAdHelper

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

- (NSString*)k__show {
    return [NSString stringWithFormat:@"%@_show_%@", prefix_, adId_];
}

- (void)registerHandlers:(id<EEInterstitialAdInterface> _Nonnull)ad {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:[self k__isLoaded]
                   callback:^(NSString* message) {
                       return [EEUtils toString:[ad isLoaded]];
                   }];

    [bridge registerHandler:[self k__load]
                   callback:^(NSString* message) {
                       [ad load];
                       return @"";
                   }];

    [bridge registerHandler:[self k__show]
                   callback:^(NSString* message) {
                       [ad show];
                       return @"";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:[self k__isLoaded]];
    [bridge deregisterHandler:[self k__load]];
    [bridge deregisterHandler:[self k__show]];
}

@end
