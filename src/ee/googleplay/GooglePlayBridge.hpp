//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_RECORDER_BRIDGE_HPP
#define EE_X_RECORDER_BRIDGE_HPP

#include <string>

#include "ee/core/LogLevel.hpp"

namespace ee {
namespace google {
class Play final {
public:
    Play();
    ~Play();

    bool isSignedIn();
    void signin(bool showLoginUI = true);
    void signout();

    void setSteps(const std::string& achievement_name, double steps);
    void showAchievements();
    void loadAchievements(bool force_reload);

    void incrementAchievement(const std::string& achievement_name,
                              int increment);
    void unlockAchievement(const std::string& achievement_name);

    void showLeaderboard(const std::string& leaderboard_name = "");
    void showAllLeaderboards();
    void submitScore(const std::string& leaderboard_name, long score);

private:
    MessageBridge& bridge_;
};
} // namespace google
} // namespace ee

#endif /* EE_X_RECORDER_BRIDGE_HPP */
