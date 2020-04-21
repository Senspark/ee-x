#import <ee/core/EEIPlugin.h>

@interface EEVungle : NSObject <EEIPlugin>

- (void)destroy;

- (void)initialize:(NSString* _Nonnull)gameId;

- (BOOL)hasRewardedAd:(NSString* _Nonnull)adId;
- (void)loadRewardedAd:(NSString* _Nonnull)adId;
- (void)showRewardedAd:(NSString* _Nonnull)adId;

@end
