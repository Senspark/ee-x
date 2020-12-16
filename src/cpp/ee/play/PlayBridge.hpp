//
//  Created by Kiet Le on 12/24/2019.
//
//

#ifndef EE_X_PLAY_BRIDGE_HPP
#define EE_X_PLAY_BRIDGE_HPP

#ifdef __cplusplus

#include <functional>
#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/play/PlayFwd.hpp"

namespace ee {
namespace play {
class Bridge final : public IPlugin {
public:
    Bridge();
    ~Bridge();

    virtual void destroy() override;

    /// Checks whether the user is logged in Play services.
    bool isLoggedIn() const;

    /// Attempts to log the user in Play services.
    [[nodiscard]] Task<bool> logIn(bool silently = false);

    /// Logs user out Play services.
    [[nodiscard]] Task<bool> logOut();

    /// Show achievements.
    void showAchievements();

    /// Increments the specified achievement.
    void incrementAchievement(const std::string& achievementId, double percent);

    /// Unlocks the specifieid achievement.
    void unlockAchievement(const std::string& achievementId);

    /// Shows the specifieid leaderboard.
    void showLeaderboard(const std::string& leaderboardId);

    /// Show all leaderboards.
    void showAllLeaderboards();

    /// Submits a leaderboard score.
    void submitScore(const std::string& leaderboardId, std::int64_t score);

private:
    IMessageBridge& bridge_;
};
} // namespace play
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_PLAY_BRIDGE_HPP */
