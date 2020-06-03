//
//  EEVideoPlayerManager.m
//  ee_x_mobile_core
//
//  Created by eps on 1/31/18.
//

#import "ee/core/private/EEVideoPlayerManager.h"

#import <ee_x-Swift.h>

#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/private/EEVideoPlayer.h"

@implementation EEVideoPlayerManager {
    EEMessageBridge* bridge_;
    NSMutableDictionary<NSString*, EEVideoPlayer*>* players_;
}

static NSString* const k__create = @"VideoPlayerManager_create";
static NSString* const k__destroy = @"VideoPlayerManager_destroy";

+ (instancetype)getInstance {
    static EEVideoPlayerManager* sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

+ (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge registerHandler:k__create
                   callback:^(NSString* message) {
                       NSString* tag = message;
                       return [EEUtils
                           toString:[[self getInstance] createVideoPlayer:tag]];
                   }];

    [bridge registerHandler:k__destroy
                   callback:^(NSString* message) {
                       NSString* tag = message;
                       return [EEUtils toString:[[self getInstance]
                                                    destroyVideoPlayer:tag]];
                   }];
}

- (id)init {
    self = [super init];
    if (self == nil) {
        return nil;
    }
    bridge_ = [EEMessageBridge getInstance];
    players_ = [[NSMutableDictionary alloc] init];
    return self;
}

- (BOOL)createVideoPlayer:(NSString* _Nonnull)tag {
    if ([players_ objectForKey:tag] != nil) {
        return NO;
    }
    EEVideoPlayer* player =
        [[[EEVideoPlayer alloc] initWithBridge:bridge_ tag:tag] autorelease];
    [players_ setObject:player forKey:tag];
    return YES;
}

- (BOOL)destroyVideoPlayer:(NSString* _Nonnull)tag {
    if ([players_ objectForKey:tag] == nil) {
        return NO;
    }
    EEVideoPlayer* player = [players_ objectForKey:tag];
    [player destroy];
    [players_ removeObjectForKey:tag];
    return YES;
}

@end
