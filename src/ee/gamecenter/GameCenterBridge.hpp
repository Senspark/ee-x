//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_GAMECENTER_BRIDGE_HPP
#define EE_X_GAMECENTER_BRIDGE_HPP

#include <string>

#include "ee/core/LogLevel.hpp"

namespace ee {
namespace game {
class GameCenter final {
public:
    GameCenter();
    ~GameCenter();

    bool isSignedIn();
    void signin(bool showLoginUI = true);
    void signout();

    void showAchievements();
    void loadAchievements(bool force_reload);

    void incrementAchievement(const std::string& achievement_id,
                              double percent);
    void unlockAchievement(const std::string& achievement_id);

    void showLeaderboard(const std::string& leaderboard_id = "");
    void showAllLeaderboards();
    void submitScore(const std::string& leaderboard_id, int64_t score);

private:
    IMessageBridge& bridge_;
};
} // namespace game
} // namespace ee

#endif /* EE_X_GAMECENTER_BRIDGE_HPP */
