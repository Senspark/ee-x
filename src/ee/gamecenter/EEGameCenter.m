//
//  Created by VinhNH on 7/2/2018.
//
//

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/gamecenter/EEGameCenter.h"

#import <GameKit/GameKit.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <netinet/in.h>
#import <sys/socket.h>

#define IS_MIN_IOS6                                                            \
    ([[[UIDevice currentDevice] systemVersion] floatValue] >= 6.0f)

// TODO: Cache all user's data to client then push all of them to store when
// signin

@interface EEGameCenter () <GKGameCenterControllerDelegate> {
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

    [self registerHandlers];

    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        return [EEUtils toString:[self isSignedIn]];
    }
                        tag:k_isSignedIn];

    [bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        BOOL showLoginUI = [dict[k_showLoginUI] boolValue];
        [self signin:showLoginUI];
        return @"";
    }
                        tag:k_signin];

    [bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        [self signout];
        return @"";
    }
                        tag:k_signout];

    [bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        [self showAchievements];
        return @"";
    }
                        tag:k_showAchievements];

    [bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* achievementId = dict[k_achievementId];
        double percent = [dict[k_increment] doubleValue];
        [self incrementAchievement:achievementId withPercent:percent];
        return @"";
    }
                        tag:k_increaseAchievement];

    [bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* achievementId = dict[k_achievementId];
        [self unlockAchievement:achievementId];
        return @"";
    }
                        tag:k_unlockAchievement];

    [bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* leaderboardId = dict[k_leaderboardId];
        [self showLeaderboard:leaderboardId];
        return @"";
    }
                        tag:k_showLeaderboard];

    [bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        [self showAllLeaderboards];
        return @"";
    }
                        tag:k_showAllLeaderboards];

    [bridge registerHandler:^NSString* _Nonnull(NSString* _Nonnull message) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
        NSString* leaderboardId = dict[k_leaderboardId];
        long long score = [dict[k_score] longLongValue];
        [self submitScore:leaderboardId withScore:score];
        return @"";
    }
                        tag:k_submitScore];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge deregisterHandler:k_isSignedIn];
    [bridge deregisterHandler:k_signin];
    [bridge deregisterHandler:k_signout];
    [bridge deregisterHandler:k_showAchievements];
    [bridge deregisterHandler:k_increaseAchievement];
    [bridge deregisterHandler:k_unlockAchievement];
    [bridge deregisterHandler:k_showLeaderboard];
    [bridge deregisterHandler:k_showAllLeaderboards];
    [bridge deregisterHandler:k_submitScore];
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

    if (IS_MIN_IOS6) {
        [player setAuthenticateHandler:authBlock];
    }
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

#pragma mark - Connectivity Checking
/*
 Connectivity testing code pulled from Apple's Reachability Example:
 http://developer.apple.com/library/ios/#samplecode/Reachability Taken from -
 http://stackoverflow.com/questions/1083701/how-to-check-for-an-active-internet-connection-on-iphone-sdk
 Leak fixed with CFRelease
 */
- (BOOL)hasConnectivity {
    struct sockaddr_in zeroAddress;
    bzero(&zeroAddress, sizeof(zeroAddress));
    zeroAddress.sin_len = sizeof(zeroAddress);
    zeroAddress.sin_family = AF_INET;

    SCNetworkReachabilityRef reachability =
        SCNetworkReachabilityCreateWithAddress(
            kCFAllocatorDefault, (const struct sockaddr*)&zeroAddress);

    if (reachability != NULL) {
        // NetworkStatus retVal = NotReachable;
        SCNetworkReachabilityFlags flags;
        if (SCNetworkReachabilityGetFlags(reachability, &flags)) {
            if ((flags & kSCNetworkReachabilityFlagsReachable) == 0) {
                // if target host is not reachable
                CFRelease(reachability);
                return NO;
            }

            if ((flags & kSCNetworkReachabilityFlagsConnectionRequired) == 0) {
                // if target host is reachable and no connection is required
                // then we'll assume (for now) that your on Wi-Fi
                CFRelease(reachability);
                return YES;
            }

            if ((((flags & kSCNetworkReachabilityFlagsConnectionOnDemand) !=
                  0) ||
                 (flags & kSCNetworkReachabilityFlagsConnectionOnTraffic) !=
                     0)) {
                // ... and the connection is on-demand (or on-traffic) if the
                // calling application is using the CFSocketStream or higher
                // APIs
                if ((flags & kSCNetworkReachabilityFlagsInterventionRequired) ==
                    0) {
                    // ... and no [user] intervention is needed
                    CFRelease(reachability);
                    return YES;
                }
            }

            if ((flags & kSCNetworkReachabilityFlagsIsWWAN) ==
                kSCNetworkReachabilityFlagsIsWWAN) {
                // ... but WWAN connections are OK if the calling application is
                // using the CFNetwork (CFSocketStream?) APIs.
                CFRelease(reachability);
                return YES;
            }
        }
    }
    // CFRelease(reachability); //Null pointer argument in call to CFRelease
    return NO;
}

#pragma mark - Utils
- (void)complain:(NSString*)message {
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Alert!"
                                                    message:message
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
}

@end
