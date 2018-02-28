//
//  EEVideoPlayer.m
//  ee_x_mobile_core
//
//  Created by eps on 1/30/18.
//

#import <MediaPlayer/MediaPlayer.h>

#import "ee/core/internal/EEVideoPlayer.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMetrics.h"
#import "ee/core/EEMessageBridge.h"

@interface EEVideoPlayer ()
@property (nonatomic, strong, nonnull) MPMoviePlayerController* moviePlayer;
@end

@implementation EEVideoPlayer {
    NSString* tag_;
}

@synthesize moviePlayer = moviePlayer_;

- (id)initWithTag:(NSString* _Nonnull)tag {
    self = [super init];
    if (self == nil) {
        return nil;
    }
    tag_ = [tag copy];

    MPMoviePlayerController* player =
        [[[MPMoviePlayerController alloc] init] autorelease];

    [player setAllowsAirPlay:NO];
    [player setControlStyle:MPMovieControlStyleNone];

    [[player backgroundView] setBackgroundColor:[UIColor clearColor]];
    [[player view] setBackgroundColor:[UIColor clearColor]];
    for (UIView* view in [[player view] subviews]) {
        [view setBackgroundColor:[UIColor clearColor]];
    }

    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(moviePlayerDidFinish:)
               name:MPMoviePlayerPlaybackDidFinishNotification
             object:player];
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(moviePlayerStateDidChange)
               name:MPMoviePlayerPlaybackStateDidChangeNotification
             object:player];

    [[[EEUtils getCurrentRootViewController] view] addSubview:[player view]];
    [self setMoviePlayer:player];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)destroy {
    [self deregisterHandlers];
    [[NSNotificationCenter defaultCenter]
        removeObserver:self
                  name:MPMoviePlayerPlaybackDidFinishNotification
                object:[self moviePlayer]];
    [[NSNotificationCenter defaultCenter]
        removeObserver:self
                  name:MPMoviePlayerPlaybackStateDidChangeNotification
                object:[self moviePlayer]];
    [[self moviePlayer] stop];
    [[[self moviePlayer] view] removeFromSuperview];
    [self setMoviePlayer:nil];
}

- (NSString* _Nonnull)k__loadFile {
    return [@"VideoPlayer_loadFile_" stringByAppendingString:tag_];
}

- (NSString* _Nonnull)k__setPosition {
    return [@"VideoPlayer_setPosition_" stringByAppendingString:tag_];
}

- (NSString* _Nonnull)k__setSize {
    return [@"VideoPlayer_setSize_" stringByAppendingString:tag_];
}

- (NSString* _Nonnull)k__play {
    return [@"VideoPlayer_play_" stringByAppendingString:tag_];
}

- (NSString* _Nonnull)k__pause {
    return [@"VideoPlayer_pause_" stringByAppendingString:tag_];
}

- (NSString* _Nonnull)k__resume {
    return [@"VideoPlayer_resume_" stringByAppendingString:tag_];
}

- (NSString* _Nonnull)k__stop {
    return [@"VideoPlayer_stop_" stringByAppendingString:tag_];
}

- (NSString* _Nonnull)k__setVisible {
    return [@"VideoPlayer_setVisible_" stringByAppendingString:tag_];
}

- (NSString* _Nonnull)k__setKeepAspectRatioEnabled {
    return [@"VideoPlayer_setKeepAspectRatioEnabled_"
        stringByAppendingString:tag_];
}

- (NSString* _Nonnull)k__setFullScreenEnabled {
    return [@"VideoPlayer_setFullScreenEnabled_" stringByAppendingString:tag_];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:[self k__loadFile]
                   callback:^(NSString* message) {
                       [self loadFile:message];
                       return @"";
                   }];
    [bridge registerHandler:[self k__setPosition]
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       int x = [dict[@"x"] intValue];
                       int y = [dict[@"y"] intValue];
                       [self setPosition:CGPointMake(x, y)];
                       return @"";
                   }];
    [bridge registerHandler:[self k__setSize]
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       int width = [dict[@"width"] intValue];
                       int height = [dict[@"height"] intValue];
                       [self setSize:CGSizeMake(width, height)];
                       return @"";
                   }];
    [bridge registerHandler:[self k__play]
                   callback:^(NSString* message) {
                       [self play];
                       return @"";
                   }];
    [bridge registerHandler:[self k__pause]
                   callback:^(NSString* message) {
                       [self pause];
                       return @"";
                   }];
    [bridge registerHandler:[self k__resume]
                   callback:^(NSString* message) {
                       [self pause];
                       return @"";
                   }];
    [bridge registerHandler:[self k__stop]
                   callback:^(NSString* message) {
                       [self resume];
                       return @"";
                   }];
    [bridge registerHandler:[self k__setVisible]
                   callback:^(NSString* message) {
                       [self setVisible:[EEUtils toBool:message]];
                       return @"";
                   }];
    [bridge
        registerHandler:[self k__setKeepAspectRatioEnabled]
               callback:^(NSString* message) {
                   [self setKeepAspectRatioEnabled:[EEUtils toBool:message]];
                   return @"";
               }];
    [bridge registerHandler:[self k__setFullScreenEnabled]
                   callback:^(NSString* message) {
                       [self setFullScreenEnabled:[EEUtils toBool:message]];
                       return @"";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:[self k__play]];
    [bridge deregisterHandler:[self k__pause]];
    [bridge deregisterHandler:[self k__resume]];
    [bridge deregisterHandler:[self k__stop]];
    [bridge deregisterHandler:[self k__setVisible]];
    [bridge deregisterHandler:[self k__setKeepAspectRatioEnabled]];
    [bridge deregisterHandler:[self k__setFullScreenEnabled]];
}

- (void)loadFile:(NSString* _Nonnull)path {
    NSURL* url = [NSURL fileURLWithPath:path];
    [[self moviePlayer] setMovieSourceType:MPMovieSourceTypeFile];
    [[self moviePlayer] setContentURL:url];
}

- (void)setPosition:(CGPoint)position {
    CGFloat scale = [EEMetrics getDensity];
    CGRect frame = [[[self moviePlayer] view] frame];
    frame.origin = CGPointMake(position.x / scale, position.y / scale);
    [[[self moviePlayer] view] setFrame:frame];
}

- (void)setSize:(CGSize)size {
    CGFloat scale = [EEMetrics getDensity];
    CGRect frame = [[[self moviePlayer] view] frame];
    frame.size = CGSizeMake(size.width / scale, size.height / scale);
    [[[self moviePlayer] view] setFrame:frame];
}

- (void)play {
    [[self moviePlayer] play];
}

- (void)pause {
    [[self moviePlayer] pause];
}

- (void)resume{
    // [[self moviePlayer] play];
}

    - (void)stop {
    [[self moviePlayer] stop];
}

- (NSTimeInterval)getCurrentPlaybackTime {
    return [[self moviePlayer] currentPlaybackTime];
}

- (void)setCurrentPlaybackTime:(NSTimeInterval)second {
    [[self moviePlayer] setCurrentPlaybackTime:second];
}

- (BOOL)isVisible {
    return ![[[self moviePlayer] view] isHidden];
}

- (void)setVisible:(BOOL)visible {
    [[[self moviePlayer] view] setHidden:!visible];
}

- (BOOL)isKeepAspectRatioEnabled {
    return [[self moviePlayer] scalingMode] == MPMovieScalingModeAspectFit;
}

- (void)setKeepAspectRatioEnabled:(BOOL)enabled {
    if (enabled) {
        [[self moviePlayer] setScalingMode:MPMovieScalingModeAspectFit];
    } else {
        [[self moviePlayer] setScalingMode:MPMovieScalingModeFill];
    }
}

- (BOOL)isFullScreenEnabled {
    return [[self moviePlayer] isFullscreen];
}

- (void)setFullScreenEnabled:(BOOL)enabled {
    [[self moviePlayer] setFullscreen:[self isFullScreenEnabled]];
}

- (void)moviePlayerDidFinish:(NSNotification*)notification {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert([self moviePlayer] != nil, @"");
}

- (void)moviePlayerStateDidChange {
    NSAssert([self moviePlayer] != nil, @"");
    NSDictionary* dict = @{
        @(MPMoviePlaybackStateStopped): @"Stopped",
        @(MPMoviePlaybackStatePlaying): @"Playing",
        @(MPMoviePlaybackStatePaused): @"Pause",
        @(MPMoviePlaybackStateInterrupted): @"Interrupted",
        @(MPMoviePlaybackStateSeekingForward): @"SeekingForward",
        @(MPMoviePlaybackStateSeekingBackward): @"SeekingBackward"
    };
    MPMoviePlaybackState state = [[self moviePlayer] playbackState];
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [dict objectForKey:@(state)]);
}

@end
