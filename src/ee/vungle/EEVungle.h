#import <Foundation/Foundation.h>

@interface EEVungle : NSObject

- (void)initialize:(NSString* _Nonnull)gameId placementId:(NSString* _Nonnull)placementId;

- (BOOL)hasRewardedVideo:(NSString* _Nonnull)placementId;

- (BOOL)showRewardedVideo:(NSString* _Nonnull)placementId;

@end
