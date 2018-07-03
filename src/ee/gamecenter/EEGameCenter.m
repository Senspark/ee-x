//
//  Created by VinhNH on 7/2/2018.
//
//

#import "ee/gamecenter/EEGameCenter.h"

#import <GameKit/GameKit.h>

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"

// TODO: Cache all user's data to client then push all of them to store when
// signin

@interface EEGameCenter () <GKGameCenterControllerDelegate> {
    id<EEIMessageBridge> _bridge;
}
@end

@implementation EEGameCenter

// clang-format off
static NSString* const k_isSignedIn          = @"GameCenter_isSignedIn";
static NSString* const k_signin              = @"GameCenter_signin";
static NSString* const k_signout             = @"GameCenter_signout";
static NSString* const k_showAchievements    = @"GameCenter_showAchievements";
static NSString* const k_increaseAchievement = @"GameCenter_increaseAchievement";
static NSString* const k_unlockAchievement   = @"GameCenter_unlockAchievement";
static NSString* const k_showLeaderboard     = @"GameCenter_showLeaderboard";
static NSString* const k_showAllLeaderboards = @"GameCenter_showAllLeaderboards";
static NSString* const k_submitScore         = @"GameCenter_submitScore";

static NSString* const k_showLoginUI   = @"show_login_ui";
static NSString* const k_achievementId = @"achievement_id";
static NSString* const k_increment     = @"increment";
static NSString* const k_leaderboardId = @"leaderboard_id";
static NSString* const k_score         = @"score";
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
                         tag:k_isSignedIn];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        BOOL showLoginUI = [dict[k_showLoginUI] boolValue];
        [self signin:showLoginUI];
        return @"";
    }
                         tag:k_signin];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        [self signout];
        return @"";
    }
                         tag:k_signout];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        [self showAchievements];
        return @"";
    }
                         tag:k_showAchievements];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* achievementId = dict[k_achievementId];
        double percent = [dict[k_increment] doubleValue];
        [self incrementAchievement:achievementId withPercent:percent];
        return @"";
    }
                         tag:k_increaseAchievement];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* achievementId = dict[k_achievementId];
        [self unlockAchievement:achievementId];
        return @"";
    }
                         tag:k_unlockAchievement];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* leaderboardId = dict[k_leaderboardId];
        [self showLeaderboard:leaderboardId];
        return @"";
    }
                         tag:k_showLeaderboard];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        [self showAllLeaderboards];
        return @"";
    }
                         tag:k_showAllLeaderboards];

    [_bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* leaderboardId = dict[k_leaderboardId];
        long long score = [dict[k_score] longLongValue];
        [self submitScore:leaderboardId withScore:score];
        return @"";
    }
                         tag:k_submitScore];
}

- (void)deregisterHandlers {
    [_bridge deregisterHandler:k_isSignedIn];
    [_bridge deregisterHandler:k_signin];
    [_bridge deregisterHandler:k_signout];
    [_bridge deregisterHandler:k_showAchievements];
    [_bridge deregisterHandler:k_increaseAchievement];
    [_bridge deregisterHandler:k_unlockAchievement];
    [_bridge deregisterHandler:k_showLeaderboard];
    [_bridge deregisterHandler:k_showAllLeaderboards];
    [_bridge deregisterHandler:k_submitScore];
}

#pragma mark - Implement Game Center
#pragma mark SignIn
- (BOOL)isSignedIn {
    return [GKLocalPlayer localPlayer].isAuthenticated;
}

- (void)signin:(BOOL)showLoginUI {
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
            }

            if (error) {
                // Login failed
            }
        };

    [player setAuthenticateHandler:authBlock];
}

- (void)signout {
    // Do nothing
}

#pragma mark Achievement
- (void)showAchievements {
    if (![self isSignedIn]) {
        [self signin:YES];
        return;
    }

    UIViewController* rootViewController =
        [EEUtils getCurrentRootViewController];
    GKGameCenterViewController* viewController =
        [[GKGameCenterViewController alloc] init];
    viewController.viewState = GKGameCenterViewControllerStateAchievements;
    viewController.gameCenterDelegate = self;

    [rootViewController presentViewController:viewController
                                     animated:YES
                                   completion:nil];
}

- (void)loadAchievements:(BOOL)force_reload {
    // Not implement
}

- (void)incrementAchievement:(NSString*)achievementId
                 withPercent:(double)percent {
    if (![self isSignedIn]) {
        return;
    }
    if (percent > 100.0f)
        percent = 100.0f;

    GKAchievement* achievement =
        [[GKAchievement alloc] initWithIdentifier:achievementId];
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
        [self signin:YES];
        return;
    }
    if (leaderboardId == nil || leaderboardId.length == 0) {
        return;
    }

    UIViewController* rootViewController =
        [EEUtils getCurrentRootViewController];
    GKGameCenterViewController* viewController =
        [[GKGameCenterViewController alloc] init];
    viewController.viewState = GKGameCenterViewControllerStateLeaderboards;
    viewController.gameCenterDelegate = self;
    viewController.leaderboardIdentifier = leaderboardId;

    [rootViewController presentViewController:viewController
                                     animated:YES
                                   completion:nil];
}

- (void)showAllLeaderboards {
    if (![self isSignedIn]) {
        [self signin:YES];
        return;
    }
    UIViewController* rootViewController =
        [EEUtils getCurrentRootViewController];
    GKGameCenterViewController* viewController =
        [[GKGameCenterViewController alloc] init];
    viewController.viewState = GKGameCenterViewControllerStateLeaderboards;
    viewController.gameCenterDelegate = self;

    [rootViewController presentViewController:viewController
                                     animated:YES
                                   completion:nil];
}

- (void)submitScore:(NSString*)leaderboardId withScore:(long long)aScore {
    if (![self isSignedIn]) {
        return;
    }
    GKScore* score =
        [[GKScore alloc] initWithLeaderboardIdentifier:leaderboardId];
    score.value = aScore;
    NSArray* scoreArr = @[score];
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
