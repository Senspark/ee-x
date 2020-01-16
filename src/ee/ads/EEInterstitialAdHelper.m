//
//  IInterstitialAd.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/ads/EEInterstitialAdHelper.h"

#import <ee/core/EEIMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

#import "ee/ads/EEIInterstitialAd.h"

@interface EEInterstitialAdHelper () {
    id<EEIMessageBridge> bridge_;
    NSString* prefix_;
    NSString* adId_;
}

@end

@implementation EEInterstitialAdHelper

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                       prefix:(NSString* _Nonnull)prefix
                         adId:(NSString* _Nonnull)adId {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
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

- (void)registerHandlers:(id<EEIInterstitialAd> _Nonnull)ad {
    [bridge_ registerHandler:[self k__isLoaded]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[ad isLoaded]];
                    }];

    [bridge_ registerHandler:[self k__load]
                    callback:^(NSString* message) {
                        [ad load];
                        return @"";
                    }];

    [bridge_ registerHandler:[self k__show]
                    callback:^(NSString* message) {
                        [ad show];
                        return @"";
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:[self k__isLoaded]];
    [bridge_ deregisterHandler:[self k__load]];
    [bridge_ deregisterHandler:[self k__show]];
}

@end
