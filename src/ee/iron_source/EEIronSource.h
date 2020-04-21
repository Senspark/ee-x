#import <ee/core/EEIPlugin.h>

@interface EEIronSource : NSObject <EEIPlugin>

- (void)destroy;

- (void)initialize:(NSString* _Nonnull)gameId;

- (BOOL)hasInterstitialAd;
- (void)loadInterstitialAd;
- (void)showInterstitialAd:(NSString* _Nonnull)adId;

- (BOOL)hasRewardedAd;
- (void)showRewardedAd:(NSString* _Nonnull)adId;

@end
