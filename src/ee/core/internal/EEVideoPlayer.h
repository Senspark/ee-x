//
//  EEVideoPlayer.h
//  ee_x
//
//  Created by eps on 1/30/18.
//

#import <CoreGraphics/CGGeometry.h>
#import <Foundation/Foundation.h>

@class MPMoviePlayerController;

@interface EEVideoPlayer : NSObject

- (id _Null_unspecified)initWithTag:(NSString* _Nonnull)tag;
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
