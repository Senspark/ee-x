import GameKit

private let kPrefix = "Play"
private let kIsLoggedIn = "\(kPrefix)IsLoggedIn"
private let kLogIn = "\(kPrefix)LogIn"
private let kLogOut = "\(kPrefix)LogOut"
private let kShowAchievements = "\(kPrefix)ShowAchievements"
private let kIncrementAchievement = "\(kPrefix)IncrementAchievement"
private let kUnlockAchievement = "\(kPrefix)UnlockAchievement"
private let kShowLeaderboard = "\(kPrefix)ShowLeaderboard"
private let kShowAllLeaderboards = "\(kPrefix)ShowAllLeaderboards"
private let kSubmitScore = "\(kPrefix)SubmitScore"

@objc(EEPlay)
public class Play: NSObject, IPlugin, GKGameCenterControllerDelegate {
    private let _bridge: IMessageBridge
    
    public required init(_ bridge: IMessageBridge) {
        _bridge = bridge
        super.init()
        registerHandlers()
    }
    
    public func destroy() {
        deregisterHandlers()
    }
    
    private func registerHandlers() {
        _bridge.registerHandler(kIsLoggedIn) { _ in
            Utils.toString(self.isLoggedIn)
        }
        _bridge.registerAsyncHandler(kLogIn) { _, resolver in
            self.logIn { successful in
                resolver(Utils.toString(successful))
            }
        }
        _bridge.registerAsyncHandler(kLogOut) { _, resolver in
            // Not supported.
            resolver(Utils.toString(false))
        }
        _bridge.registerHandler(kShowAchievements) { _ in
            self.showAchievements()
            return ""
        }
        _bridge.registerHandler(kIncrementAchievement) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let achievementId = dict["achievement_id"] as? String else {
                assert(false, "Invalid argument")
                return ""
            }
            guard let increment = dict["increment"] as? Double else {
                assert(false, "Invalid argument")
                return ""
            }
            self.incrementAchievement(achievementId, increment)
            return ""
        }
        _bridge.registerHandler(kUnlockAchievement) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let achievementId = dict["achievement_id"] as? String else {
                assert(false, "Invalid argument")
                return ""
            }
            self.unlockAchievement(achievementId)
            return ""
        }
        _bridge.registerHandler(kShowLeaderboard) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let leaderboardId = dict["leaderboard_id"] as? String else {
                assert(false, "Invalid argument")
                return ""
            }
            self.showLeaderboard(leaderboardId)
            return ""
        }
        _bridge.registerHandler(kShowAllLeaderboards) { _ in
            self.showAllLeaderboards()
            return ""
        }
        _bridge.registerHandler(kSubmitScore) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let leaderboardId = dict["leaderboard_id"] as? String else {
                assert(false, "Invalid argument")
                return ""
            }
            guard let score = dict["score"] as? Int64 else {
                assert(false, "Invalid argument")
                return ""
            }
            self.submitScore(leaderboardId, score)
            return ""
        }
    }
    
    private func deregisterHandlers() {
        _bridge.deregisterHandler(kIsLoggedIn)
        _bridge.deregisterHandler(kLogIn)
        _bridge.deregisterHandler(kLogOut)
        _bridge.deregisterHandler(kShowAchievements)
        _bridge.deregisterHandler(kIncrementAchievement)
        _bridge.deregisterHandler(kUnlockAchievement)
        _bridge.deregisterHandler(kShowLeaderboard)
        _bridge.deregisterHandler(kShowAllLeaderboards)
        _bridge.deregisterHandler(kSubmitScore)
    }
    
    private var isLoggedIn: Bool {
        return GKLocalPlayer.local.isAuthenticated
    }
    
    private func logIn(_ callback: @escaping (_ successful: Bool) -> Void) {
        GKLocalPlayer.local.authenticateHandler = { viewController, error in
            if let vc = viewController {
                Utils.getCurrentRootViewController()?.present(vc, animated: true) {
                    // Done.
                }
            }
            if GKLocalPlayer.local.isAuthenticated {
                callback(true)
                GKLocalPlayer.local.authenticateHandler = nil
                return
            }
            if error != nil {
                callback(false)
                GKLocalPlayer.local.authenticateHandler = nil
            }
        }
    }
    
    private func showAchievements() {
        if !isLoggedIn {
            return
        }
        let viewController = GKGameCenterViewController()
        viewController.viewState = GKGameCenterViewControllerState.achievements
        viewController.gameCenterDelegate = self
        
        Utils.getCurrentRootViewController()?.present(viewController, animated: true, completion: nil)
    }
    
    private func incrementAchievement(_ achievementId: String, _ percent: Double) {
        if !isLoggedIn {
            return
        }
        let achievement = GKAchievement(identifier: achievementId)
        achievement.percentComplete = min(percent, 100)
        GKAchievement.report([achievement], withCompletionHandler: { _ in
            // OK.
        })
    }
    
    private func unlockAchievement(_ achievementId: String) {
        incrementAchievement(achievementId, 100)
    }
    
    private func showLeaderboard(_ leaderboardId: String) {
        if !isLoggedIn {
            return
        }
        let viewController = GKGameCenterViewController()
        viewController.viewState = GKGameCenterViewControllerState.leaderboards
        viewController.leaderboardIdentifier = leaderboardId
        viewController.gameCenterDelegate = self
        
        Utils.getCurrentRootViewController()?.present(viewController, animated: true, completion: nil)
    }
    
    private func showAllLeaderboards() {
        if !isLoggedIn {
            return
        }
        let viewController = GKGameCenterViewController()
        viewController.viewState = GKGameCenterViewControllerState.leaderboards
        viewController.gameCenterDelegate = self
        
        Utils.getCurrentRootViewController()?.present(viewController, animated: true, completion: nil)
    }
    
    private func submitScore(_ leaderboardId: String, _ score: Int64) {
        let item = GKScore(leaderboardIdentifier: leaderboardId)
        item.value = score
        GKScore.report([item], withCompletionHandler: { _ in
            // OK
        })
    }
    
    public func gameCenterViewControllerDidFinish(_ gameCenterViewController: GKGameCenterViewController) {
        gameCenterViewController.dismiss(animated: true, completion: nil)
    }
}
