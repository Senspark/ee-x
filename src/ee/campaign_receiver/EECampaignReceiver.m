//
//  EECampaignReceiver.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/campaign_receiver/EECampaignReceiver.h"

#import <ee-Swift.h>

#import <ee/core/internal/EEJsonUtils.h>

@implementation EECampaignReceiver

// clang-format off
static NSString* const kOnReceivedLink = @"CampaignReceiverBridgeOnReceivedLink";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }

    return self;
}

- (void)dealloc {
    [super dealloc];
}

+ (BOOL)application:(UIApplication*)app
            openURL:(nonnull NSURL*)url
            options:(nonnull NSDictionary<NSString*, id>*)options {
    id<EEIMessageBridge> bridgeMessage = [EEMessageBridge getInstance];
    [bridgeMessage callCpp:kOnReceivedLink //
                          :[url absoluteString]];
    return true;
}
@end
