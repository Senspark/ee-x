#import <Foundation/Foundation.h>

@interface EEIronSource : NSObject

- (void)initialize:(NSString* _Nonnull)gameId;

- (BOOL)isRewardedVideoReady;

- (BOOL)showRewardedVideo:(NSString* _Nonnull)placementId;

@end
