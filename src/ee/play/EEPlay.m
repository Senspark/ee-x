//
//  EEPlay.m
//
//  Created by Kiet Le on 12/24/2019.
//
//

#import "ee/play/EEPlay.h"

#import <GameKit/GameKit.h>

#import <ee_x-Swift.h>

#import <ee/core/internal/EEJsonUtils.h>

@interface EEPlay () <GKGameCenterControllerDelegate> {
    id<EEIMessageBridge> _bridge;
}
@end

@implementation EEPlay

// clang-format off
static NSString* const k__isSignedIn          = @"Play_isSignedIn";
static NSString* const k__signin              = @"Play_signin";
static NSString* const k__signout             = @"Play_signout";
static NSString* const k__showAchievements    = @"Play_showAchievements";
static NSString* const k__increaseAchievement = @"Play_increaseAchievement";
static NSString* const k__unlockAchievement   = @"Play_unlockAchievement";
static NSString* const k__showLeaderboard     = @"Play_showLeaderboard";
static NSString* const k__showAllLeaderboards = @"Play_showAllLeaderboards";
static NSString* const k__submitScore         = @"Play_submitScore";
static NSString* const k_onSignedIn           = @"Play_onSignedIn";

static NSString* const k__achievementId = @"achievement_id";
static NSString* const k__increment     = @"increment";
static NSString* const k__leaderboardId = @"leaderboard_id";
static NSString* const k__score         = @"score";
// clang-format on

- (id)init {
    self = [super init];
    _bridge = [EEMessageBridge getInstance];
    [self registerHandlers];

    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    [_bridge registerHandler:
               k__isSignedIn:^NSString* _Nonnull(NSString* _Nonnull message) {
                   return [EEUtils toString:[self isSignedIn]];
               }];

    [_bridge registerHandler:
                   k__signin:^NSString* _Nonnull(NSString* _Nonnull message) {
                       [self signIn];
                       return @"";
                   }];

    [_bridge registerHandler:
                  k__signout:^NSString* _Nonnull(NSString* _Nonnull message) {
                      [self signOut];
                      return @"";
                  }];

    [_bridge registerHandler:
         k__showAchievements:^NSString* _Nonnull(NSString* _Nonnull message) {
             [self showAchievements];
             return @"";
         }];

    [_bridge registerHandler:
        k__increaseAchievement:^NSString* _Nonnull(NSString* _Nonnull message) {
            NSDictionary* dict =
                [EEJsonUtils convertStringToDictionary:message];
            NSString* achievementId = dict[k__achievementId];
            double percent = [dict[k__increment] doubleValue];
            [self incrementAchievement:achievementId withPercent:percent];
            return @"";
        }];

    [_bridge registerHandler:
        k__unlockAchievement:^NSString* _Nonnull(NSString* _Nonnull message) {
            NSDictionary* dict =
                [EEJsonUtils convertStringToDictionary:message];
            NSString* achievementId = dict[k__achievementId];
            [self unlockAchievement:achievementId];
            return @"";
        }];

    [_bridge registerHandler:
          k__showLeaderboard:^NSString* _Nonnull(NSString* _Nonnull message) {
              NSDictionary* dict =
                  [EEJsonUtils convertStringToDictionary:message];
              NSString* leaderboardId = dict[k__leaderboardId];
              [self showLeaderboard:leaderboardId];
              return @"";
          }];

    [_bridge registerHandler:
        k__showAllLeaderboards:^NSString* _Nonnull(NSString* _Nonnull message) {
            [self showAllLeaderboards];
            return @"";
        }];

    [_bridge registerHandler:
              k__submitScore:^NSString* _Nonnull(NSString* _Nonnull message) {
                  NSDictionary* dict =
                      [EEJsonUtils convertStringToDictionary:message];
                  NSString* leaderboardId = dict[k__leaderboardId];
                  int64_t score = [dict[k__score] longLongValue];
                  [self submitScore:leaderboardId withScore:score];
                  return @"";
              }];
}

- (void)deregisterHandlers {
    [_bridge deregisterHandler:k__isSignedIn];
    [_bridge deregisterHandler:k__signin];
    [_bridge deregisterHandler:k__signout];
    [_bridge deregisterHandler:k__showAchievements];
    [_bridge deregisterHandler:k__increaseAchievement];
    [_bridge deregisterHandler:k__unlockAchievement];
    [_bridge deregisterHandler:k__showLeaderboard];
    [_bridge deregisterHandler:k__showAllLeaderboards];
    [_bridge deregisterHandler:k__submitScore];
}

#pragma mark - Implement Game Center
#pragma mark SignIn
- (BOOL)isSignedIn {
    return [GKLocalPlayer localPlayer].isAuthenticated;
}

- (void)signIn {
    GKLocalPlayer* player = [GKLocalPlayer localPlayer];

    void (^authBlock)(UIViewController*, NSError*) =
        ^(UIViewController* viewController, NSError* error) {
            if (viewController) {
                UIViewController* rootViewController =
                    [EEUtils getCurrentRootViewController];
                [rootViewController presentViewController:viewController
                                                 animated:YES
                                               completion:nil];
            }

            if ([[GKLocalPlayer localPlayer] isAuthenticated]) {
                [_bridge callCpp:k_onSignedIn:[EEUtils toString:YES]];
            } else {
                [_bridge callCpp:k_onSignedIn:[EEUtils toString:NO]];
            }
        };

    [player setAuthenticateHandler:authBlock];
}

- (void)signOut {
}

#pragma mark Achievement
- (void)showAchievements {
    if (![self isSignedIn]) {
        [self signIn];
        return;
    }

    GKGameCenterViewController* viewController =
        [[[GKGameCenterViewController alloc] init] autorelease];
    viewController.viewState = GKGameCenterViewControllerStateAchievements;
    viewController.gameCenterDelegate = self;

    UIViewController* rootViewController =
        [EEUtils getCurrentRootViewController];
    [rootViewController presentViewController:viewController
                                     animated:YES
                                   completion:nil];
}

- (void)loadAchievements:(BOOL)forceReload {
}

- (void)incrementAchievement:(NSString*)achievementId
                 withPercent:(double)percent {
    if (![self isSignedIn]) {
        return;
    }
    if (percent > 100.0f) {
        percent = 100.0f;
    }

    GKAchievement* achievement =
        [[[GKAchievement alloc] initWithIdentifier:achievementId] autorelease];
    if (achievement) {
        achievement.percentComplete = percent;
        [GKAchievement reportAchievements:@[achievement]
                    withCompletionHandler:^(NSError* _Nullable error){
                    }];
    }
}

- (void)unlockAchievement:(NSString*)achievementId {
    if (![self isSignedIn]) {
        return;
    }
    [self incrementAchievement:achievementId withPercent:100];
}

#pragma mark Leaderboard
- (void)showLeaderboard:(NSString*)leaderboardId {
    if (![self isSignedIn]) {
        [self signIn];
        return;
    }
    if (leaderboardId == nil || leaderboardId.length == 0) {
        return;
    }

    UIViewController* rootViewController =
        [EEUtils getCurrentRootViewController];
    GKGameCenterViewController* viewController =
        [[[GKGameCenterViewController alloc] init] autorelease];
    viewController.viewState = GKGameCenterViewControllerStateLeaderboards;
    viewController.gameCenterDelegate = self;
    viewController.leaderboardIdentifier = leaderboardId;

    [rootViewController presentViewController:viewController
                                     animated:YES
                                   completion:nil];
}

- (void)showAllLeaderboards {
    if (![self isSignedIn]) {
        [self signIn];
        return;
    }
    UIViewController* rootViewController =
        [EEUtils getCurrentRootViewController];
    GKGameCenterViewController* viewController =
        [[[GKGameCenterViewController alloc] init] autorelease];
    viewController.viewState = GKGameCenterViewControllerStateLeaderboards;
    viewController.gameCenterDelegate = self;

    [rootViewController presentViewController:viewController
                                     animated:YES
                                   completion:nil];
}

- (void)submitScore:(NSString*)leaderboardId withScore:(int64_t)score {
    if (![self isSignedIn]) {
        return;
    }
    GKScore* gkscore = [[[GKScore alloc]
        initWithLeaderboardIdentifier:leaderboardId] autorelease];
    gkscore.value = score;
    NSArray* scoreArr = @[gkscore];
    [GKScore reportScores:scoreArr
        withCompletionHandler:^(NSError* _Nullable error){

        }];
}

#pragma mark - Override
- (void)gameCenterViewControllerDidFinish:
    (GKGameCenterViewController*)gameCenterViewController {
    [gameCenterViewController dismissViewControllerAnimated:YES completion:nil];
}

@end
