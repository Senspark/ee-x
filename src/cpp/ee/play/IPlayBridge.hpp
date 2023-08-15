#ifndef EE_X_I_PLAY_BRIDGE_HPP
#define EE_X_I_PLAY_BRIDGE_HPP

#ifdef __cplusplus

#include <functional>
#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/play/PlayFwd.hpp"

namespace ee {
namespace play {
class IBridge : public IPlugin {
public:
    /// Checks whether the user is logged in Play services.
    virtual bool isLoggedIn() const = 0;

    /// Attempts to log the user in Play services.
    [[nodiscard]] virtual Task<bool> logIn(bool silently = false) = 0;

    /// Logs user out Play services.
    [[nodiscard]] virtual Task<bool> logOut() = 0;

    /// Show achievements.
    virtual void showAchievements() = 0;

    /// Increments the specified achievement.
    virtual void incrementAchievement(const std::string& achievementId,
                                      double percent) = 0;

    /// Unlocks the specifieid achievement.
    virtual void unlockAchievement(const std::string& achievementId) = 0;

    /// Shows the specifieid leaderboard.
    virtual void showLeaderboard(const std::string& leaderboardId) = 0;

    /// Show all leaderboards.
    virtual void showAllLeaderboards() = 0;

    /// Submits a leaderboard score.
	virtual void submitScore(const std::string &leaderboardId,
							 std::int64_t score) = 0;

	/// Push save game data to cloud.
	[[nodiscard]] virtual Task<bool> pushToCloud(std::shared_ptr<ICloudDataHandler> handler, std::string title) = 0;

	/// Pull save game data from cloud.
	[[nodiscard]] virtual Task<std::shared_ptr<ICloudDataHandler>> pullFromCloud() = 0;

	/// Push save game data to cloud.
	[[nodiscard]] virtual Task<bool> deleteCloud() = 0;
};
} // namespace play
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_PLAY_BRIDGE_HPP