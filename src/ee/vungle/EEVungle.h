#import <Foundation/Foundation.h>

@interface EEVungle : NSObject

- (void)initialize:(NSString* _Nonnull)gameId;

- (BOOL)hasRewardedVideo;

- (BOOL)showRewardedVideo;

@end
