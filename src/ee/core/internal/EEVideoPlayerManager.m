//
//  EEVideoPlayerManager.m
//  ee_x_mobile_core
//
//  Created by eps on 1/31/18.
//

#import "ee/core/internal/EEVideoPlayerManager.h"
#import "ee/core/internal/EEVideoPlayer.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/core/EEMessageBridge.h"

@implementation EEVideoPlayerManager {
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
    players_ = [[NSMutableDictionary alloc] init];
    return self;
}

- (BOOL)createVideoPlayer:(NSString* _Nonnull)tag {
    if ([players_ objectForKey:tag] != nil) {
        return NO;
    }
    EEVideoPlayer* player =
        [[[EEVideoPlayer alloc] initWithTag:tag] autorelease];
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
