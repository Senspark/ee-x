//
//  IInterstitialAd.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/ads/internal/EEInterstitialAdHelper.h"

#import <ee/core/internal/EEIMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

#import "ee/ads/internal/EEIInterstitialAd.h"
#import "ee/ads/internal/EEMessageHelper.h"

@implementation EEInterstitialAdHelper {
    id<EEIMessageBridge> bridge_;
    id<EEIInterstitialAd> ad_;
    EEMessageHelper* helper_;
}

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                           ad:(id<EEIInterstitialAd> _Nonnull)ad
                       helper:(EEMessageHelper* _Nonnull)helper {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    ad_ = ad;
    helper_ = [helper retain];
    return self;
}

- (void)dealloc {
    [helper_ release];
    helper_ = nil;
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:[helper_ isLoaded]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[ad_ isLoaded]];
                    }];

    [bridge_ registerHandler:[helper_ load]
                    callback:^(NSString* message) {
                        [ad_ load];
                        return @"";
                    }];

    [bridge_ registerHandler:[helper_ show]
                    callback:^(NSString* message) {
                        [ad_ show];
                        return @"";
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:[helper_ isLoaded]];
    [bridge_ deregisterHandler:[helper_ load]];
    [bridge_ deregisterHandler:[helper_ show]];
}

@end
