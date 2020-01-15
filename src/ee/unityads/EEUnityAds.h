//
//  EEUnityAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <ee/core/EEIPlugin.h>

@interface EEUnityAds : NSObject <EEIPlugin>

- (void)initialize:(NSString* _Nonnull)gameId testMode:(BOOL)testModeEnabled;
- (void)setDebugMode:(BOOL)enabled;

- (BOOL)isRewardedVideoReady:(NSString* _Nonnull)placementId;
- (void)showRewardedVideo:(NSString* _Nonnull)placementId;

@end
