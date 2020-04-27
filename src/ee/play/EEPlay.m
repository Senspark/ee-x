//
//  EEPlay.m
//
//  Created by Kiet Le on 12/24/2019.
//
//

#import "ee/play/EEPlay.h"

#import <GameKit/GameKit.h>

#import <ee/core/internal/EEJsonUtils.h>
#import <ee/core/internal/EEMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

@interface EEPlay () <GKGameCenterControllerDelegate> {
    EEMessageBridge* _bridge;
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
    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        return [EEUtils toString:[self isSignedIn]];
    }
                         tag:k__isSignedIn];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        [self signIn];
        return @"";
    }
                         tag:k__signin];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        [self signOut];
        return @"";
    }
                         tag:k__signout];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        [self showAchievements];
        return @"";
    }
                         tag:k__showAchievements];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* achievementId = dict[k__achievementId];
        double percent = [dict[k__increment] doubleValue];
        [self incrementAchievement:achievementId withPercent:percent];
        return @"";
    }
                         tag:k__increaseAchievement];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* achievementId = dict[k__achievementId];
        [self unlockAchievement:achievementId];
        return @"";
    }
                         tag:k__unlockAchievement];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* leaderboardId = dict[k__leaderboardId];
        [self showLeaderboard:leaderboardId];
        return @"";
    }
                         tag:k__showLeaderboard];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        [self showAllLeaderboards];
        return @"";
    }
                         tag:k__showAllLeaderboards];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* leaderboardId = dict[k__leaderboardId];
        int64_t score = [dict[k__score] longLongValue];
        [self submitScore:leaderboardId withScore:score];
        return @"";
    }
                         tag:k__submitScore];
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
                [_bridge callCpp:k_onSignedIn message:[EEUtils toString:YES]];
            } else {
                [_bridge callCpp:k_onSignedIn message:[EEUtils toString:NO]];
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
