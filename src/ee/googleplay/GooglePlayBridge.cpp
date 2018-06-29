//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"
#include "ee/googleplay/GooglePlayBridge.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace google {
namespace {
// clang-format off
	constexpr auto k__isSignedIn           = "GooglePlay_isSignedIn";
	constexpr auto k_signin                = "GooglePlay_signin";
    constexpr auto k_signout               = "GooglePlay_signout";
    constexpr auto k_showAchievements      = "GooglePlay_showAchievements";
    constexpr auto k_increaseAchievement   = "GooglePlay_increaseAchievement";
    constexpr auto k_unlockAchievement     = "GooglePlay_unlockAchievement";
    constexpr auto k_showLeaderboard       = "GooglePlay_showLeaderboard";
    constexpr auto k_showAllLeaderboards   = "GooglePlay_showAllLeaderboards";
    constexpr auto k_submitScore           = "GooglePlay_submitScore";
// clang-format on
} // namespace

namespace {
// clang-format off
    constexpr auto k_showLoginUI           = "show_login_ui";
    constexpr auto k_achievementId         = "achievement_id";
    constexpr auto k_increment             = "increment";
    constexpr auto k_leaderboardId         = "leaderboard_id";
    constexpr auto k_score                 = "score";
// clang-format on
} // namespace

using Self = Play;

Self::Play()
    : bridge_(MessageBridge::getInstance()) {
    //
}

Self::~Play() {
    //
}

bool Self::isSignedIn() {
    auto response = bridge_.call(k__isSignedIn);
    return core::toBool(response);
}

void Self::signin(bool showLoginUI) {
    nlohmann::json json;
    json[k_showLoginUI] = showLoginUI;
    bridge_.call(k_signin, json.dump());
}

void Self::signout() {
    bridge_.call(k_signout);
}

void Self::setSteps(const std::string& achievement_name, double steps) {
    // Not implement
}

void Self::showAchievements() {
    bridge_.call(k_showAchievements);
}

void Self::loadAchievements(bool force_reload) {
    // Not implement
}

void Self::incrementAchievement(const std::string& achievement_name,
                                int increment) {
    nlohmann::json json;
    json[k_achievementId] = achievement_name;
    json[k_increment] = increment;
    bridge_.call(k_increaseAchievement, json.dump());
}

void Self::unlockAchievement(const std::string& achievement_name) {
    nlohmann::json json;
    json[k_achievementId] = achievement_name;
    bridge_.call(k_unlockAchievement, json.dump());
}

void Self::showLeaderboard(const std::string& leaderboard_name) {
    nlohmann::json json;
    json[k_leaderboardId] = leaderboard_name;
    bridge_.call(k_showLeaderboard, json.dump());
}

void Self::showAllLeaderboards() {
    bridge_.call(k_showAllLeaderboards);
}

void Self::submitScore(const std::string& leaderboard_name, long score) {
    nlohmann::json json;
    json[k_leaderboardId] = leaderboard_name;
    json[k_score] = score;
    bridge_.call(k_submitScore, json.dump());
}
} // namespace Google
} // namespace ee
