//
//  Created by Kiet Le on 12/24/2019.
//
//

#ifndef EE_X_PLAY_BRIDGE_HPP
#define EE_X_PLAY_BRIDGE_HPP

#include <functional>
#include <string>

#include "ee/PlayFwd.hpp"

namespace ee {
namespace play {
using LoginResultCallback = std::function<void(bool success)>;

class Bridge final {
public:
    Bridge();
    ~Bridge();

    bool isSignedIn();
    void signin(bool silentSignIn = false);
    void signout();
    void setLoginCallback(const LoginResultCallback& callback);
    void onSignedIn(bool success);

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
    LoginResultCallback callback_{nullptr};
};
} // namespace play
} // namespace ee

#endif /* EE_X_PLAY_BRIDGE_HPP */
