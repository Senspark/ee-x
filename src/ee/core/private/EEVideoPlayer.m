//
//  EEVideoPlayer.m
//  ee_x_mobile_core
//
//  Created by eps on 1/30/18.
//

#import "ee/core/private/EEVideoPlayer.h"

#import <MediaPlayer/MediaPlayer.h>

#import <ee_x-Swift.h>

#import "ee/core/internal/EEJsonUtils.h"

@interface EEVideoPlayer ()
#if TARGET_OS_IOS
@property (nonatomic, strong, nonnull) MPMoviePlayerController* moviePlayer;
#endif // TARGET_OS_IOS
@end

@implementation EEVideoPlayer {
    NSString* tag_;
    id<EEIMessageBridge> bridge_;
}

#if TARGET_OS_IOS
@synthesize moviePlayer = moviePlayer_;
#endif // TARGET_OS_IOS

- (id)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                 tag:(NSString* _Nonnull)tag {
    self = [super init];
    if (self == nil) {
        return nil;
    }
    tag_ = [tag copy];
    bridge_ = bridge;

#if TARGET_OS_IOS
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
#endif // TARGET_OS_IOS

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)destroy {
    [self deregisterHandlers];

#if TARGET_OS_IOS
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
#endif // TARGET_OS_IOS
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
    [bridge_ registerHandler:[self k__loadFile]:^(NSString* message) {
        [self loadFile:message];
        return @"";
    }];
    [bridge_ registerHandler:[self k__setPosition]:^(NSString* message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        int x = [dict[@"x"] intValue];
        int y = [dict[@"y"] intValue];
        [self setPosition:CGPointMake(x, y)];
        return @"";
    }];
    [bridge_ registerHandler:[self k__setSize]:^(NSString* message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        int width = [dict[@"width"] intValue];
        int height = [dict[@"height"] intValue];
        [self setSize:CGSizeMake(width, height)];
        return @"";
    }];
    [bridge_ registerHandler:[self k__play]:^(NSString* message) {
        [self play];
        return @"";
    }];
    [bridge_ registerHandler:[self k__pause]:^(NSString* message) {
        [self pause];
        return @"";
    }];
    [bridge_ registerHandler:[self k__resume]:^(NSString* message) {
        [self pause];
        return @"";
    }];
    [bridge_ registerHandler:[self k__stop]:^(NSString* message) {
        [self resume];
        return @"";
    }];
    [bridge_ registerHandler:[self k__setVisible]:^(NSString* message) {
        [self setVisible:[EEUtils toBool:message]];
        return @"";
    }];
    [bridge_
        registerHandler:[self k__setKeepAspectRatioEnabled] //
                       :^(NSString* message) {
                           [self setKeepAspectRatioEnabled:[EEUtils
                                                               toBool:message]];
                           return @"";
                       }];
    [bridge_
        registerHandler:[self k__setFullScreenEnabled]:^(NSString* message) {
            [self setFullScreenEnabled:[EEUtils toBool:message]];
            return @"";
        }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:[self k__play]];
    [bridge_ deregisterHandler:[self k__pause]];
    [bridge_ deregisterHandler:[self k__resume]];
    [bridge_ deregisterHandler:[self k__stop]];
    [bridge_ deregisterHandler:[self k__setVisible]];
    [bridge_ deregisterHandler:[self k__setKeepAspectRatioEnabled]];
    [bridge_ deregisterHandler:[self k__setFullScreenEnabled]];
}

- (void)loadFile:(NSString* _Nonnull)path {
#if TARGET_OS_IOS
    NSURL* url = [NSURL fileURLWithPath:path];
    [[self moviePlayer] setMovieSourceType:MPMovieSourceTypeFile];
    [[self moviePlayer] setContentURL:url];
#endif // TARGET_OS_IOS
}

- (void)setPosition:(CGPoint)position {
#if TARGET_OS_IOS
    CGFloat scale = [EEPlatformUtils getDensity];
    CGRect frame = [[[self moviePlayer] view] frame];
    frame.origin = CGPointMake(position.x / scale, position.y / scale);
    [[[self moviePlayer] view] setFrame:frame];
#endif // TARGET_OS_IOS
}

- (void)setSize:(CGSize)size {
#if TARGET_OS_IOS
    CGFloat scale = [EEPlatformUtils getDensity];
    CGRect frame = [[[self moviePlayer] view] frame];
    frame.size = CGSizeMake(size.width / scale, size.height / scale);
    [[[self moviePlayer] view] setFrame:frame];
#endif // TARGET_OS_IOS
}

- (void)play {
#if TARGET_OS_IOS
    [[self moviePlayer] play];
#endif // TARGET_OS_IOS
}

- (void)pause {
#if TARGET_OS_IOS
    [[self moviePlayer] pause];
#endif // TARGET_OS_IOS
}

- (void)resume {
    // [[self moviePlayer] play];
}

- (void)stop {
#if TARGET_OS_IOS
    [[self moviePlayer] stop];
#endif // TARGET_OS_IOS
}

- (NSTimeInterval)getCurrentPlaybackTime {
#if TARGET_OS_IOS
    return [[self moviePlayer] currentPlaybackTime];
#else  // TARGET_OS_IOS
    return 0;
#endif // TARGET_OS_IOS
}

- (void)setCurrentPlaybackTime:(NSTimeInterval)second {
#if TARGET_OS_IOS
    [[self moviePlayer] setCurrentPlaybackTime:second];
#endif // TARGET_OS_IOS
}

- (BOOL)isVisible {
#if TARGET_OS_IOS
    return ![[[self moviePlayer] view] isHidden];
#else  // TARGET_OS_IOS
    return NO;
#endif // TARGET_OS_IOS
}

- (void)setVisible:(BOOL)visible {
#if TARGET_OS_IOS
    [[[self moviePlayer] view] setHidden:!visible];
#endif // TARGET_OS_IOS
}

- (BOOL)isKeepAspectRatioEnabled {
#if TARGET_OS_IOS
    return [[self moviePlayer] scalingMode] == MPMovieScalingModeAspectFit;
#else  // TARGET_OS_IOS
    return NO;
#endif // TARGET_OS_IOS
}

- (void)setKeepAspectRatioEnabled:(BOOL)enabled {
#if TARGET_OS_IOS
    if (enabled) {
        [[self moviePlayer] setScalingMode:MPMovieScalingModeAspectFit];
    } else {
        [[self moviePlayer] setScalingMode:MPMovieScalingModeFill];
    }
#endif // TARGET_OS_IOS
}

- (BOOL)isFullScreenEnabled {
#if TARGET_OS_IOS
    return [[self moviePlayer] isFullscreen];
#else  // TARGET_OS_IOS
    return NO;
#endif // TARGET_OS_IOS
}

- (void)setFullScreenEnabled:(BOOL)enabled {
#if TARGET_OS_IOS
    [[self moviePlayer] setFullscreen:[self isFullScreenEnabled]];
#endif // TARGET_OS_IOS
}

- (void)moviePlayerDidFinish:(NSNotification*)notification {
    NSLog(@"%s", __PRETTY_FUNCTION__);
#if TARGET_OS_IOS
    NSAssert([self moviePlayer] != nil, @"");
#endif // TARGET_OS_IOS
}

- (void)moviePlayerStateDidChange {
#if TARGET_OS_IOS
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
#endif // TARGET_OS_IOS
}

@end
