//
//  EEPlay.h
//  ee_core
//
//  Created by Kiet Le on 12/24/2019.
//
//
#import <Foundation/Foundation.h>

@protocol EEIPlugin;

@interface EEPlay : NSObject <EEIPlugin>

- (void)signIn;

- (BOOL)isSignedIn;

- (void)signOut;

- (void)showAchievements;

- (void)loadAchievements:(BOOL)forceReload;

- (void)incrementAchievement:(NSString*)achievementId
                 withPercent:(double)percent;

- (void)unlockAchievement:(NSString*)achievementId;

- (void)showLeaderboard:(NSString*)leaderboardId;

- (void)showAllLeaderboards;

- (void)submitScore:(NSString*)leaderboardId withScore:(int64_t)score;

@end
