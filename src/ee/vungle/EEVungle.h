#import "ee/core/EEIPlugin.h"

@interface EEVungle : NSObject <EEIPlugin>

- (void)initialize:(NSString* _Nonnull)gameId;

- (BOOL)hasRewardedVideo:(NSString* _Nonnull)placementId;

- (BOOL)showRewardedVideo:(NSString* _Nonnull)placementId;

@end
