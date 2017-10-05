#import <Foundation/Foundation.h>

@interface EEVungle : NSObject

- (void)initialize:(NSString* _Nonnull)gameId
      placementIds:(NSArray* _Nonnull)placementIds;

- (BOOL)isAdReady:(NSString* _Nonnull)placementId;

- (BOOL)showAds:(NSString* _Nonnull)placementId;

@end
