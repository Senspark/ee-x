//
//  EEVideoPlayer.h
//  ee_x
//
//  Created by eps on 1/30/18.
//

#import <CoreGraphics/CGGeometry.h>
#import <Foundation/Foundation.h>

#if TARGET_OS_IOS
@class MPMoviePlayerController;
#endif // TARGET_OS_IOS

@protocol EEIMessageBridge;

@interface EEVideoPlayer : NSObject

- (id _Null_unspecified)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                                   tag:(NSString* _Nonnull)tag;

- (void)destroy;

- (void)loadFile:(NSString* _Nonnull)path;

- (void)setPosition:(CGPoint)position;
- (void)setSize:(CGSize)size;

- (void)play;
- (void)pause;
- (void)resume;
- (void)stop;

- (NSTimeInterval)getCurrentPlaybackTime;
- (void)setCurrentPlaybackTime:(NSTimeInterval)second;

- (BOOL)isVisible;
- (void)setVisible:(BOOL)visible;

- (BOOL)isKeepAspectRatioEnabled;
- (void)setKeepAspectRatioEnabled:(BOOL)enabled;

- (BOOL)isFullScreenEnabled;
- (void)setFullScreenEnabled:(BOOL)enabled;

@end
