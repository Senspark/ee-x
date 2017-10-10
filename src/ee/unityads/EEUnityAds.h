//
//  EEUnityAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <Foundation/Foundation.h>

@interface EEUnityAds : NSObject

- (void)initialize:(NSString* _Nonnull)gameId testMode:(BOOL)testModeEnabled;
- (void)setDebugMode:(BOOL)enabled;

- (BOOL)isRewardedVideoReady:(NSString* _Nonnull)placementId;
- (BOOL)showRewardedVideo:(NSString* _Nonnull)placementId;

@end
