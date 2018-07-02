//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"
#include "ee/gamecenter/GameCenterBridge.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace game {
namespace {
// clang-format off
constexpr auto k__isSignedIn           = "GameCenter_isSignedIn";
constexpr auto k_signin                = "GameCenter_signin";
constexpr auto k_signout               = "GameCenter_signout";
constexpr auto k_showAchievements      = "GameCenter_showAchievements";
constexpr auto k_increaseAchievement   = "GameCenter_increaseAchievement";
constexpr auto k_unlockAchievement     = "GameCenter_unlockAchievement";
constexpr auto k_showLeaderboard       = "GameCenter_showLeaderboard";
constexpr auto k_showAllLeaderboards   = "GameCenter_showAllLeaderboards";
constexpr auto k_submitScore           = "GameCenter_submitScore";
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

using Self = GameCenter;

Self::GameCenter()
    : bridge_(MessageBridge::getInstance()) {
    //
}

Self::~GameCenter() {
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

void Self::showAchievements() {
    bridge_.call(k_showAchievements);
}

void Self::loadAchievements(bool force_reload) {
    // Not implement
}

void Self::incrementAchievement(const std::string& achievement_id,
                                double percent) {
    nlohmann::json json;
    json[k_achievementId] = achievement_id;
    json[k_increment] = percent;
    bridge_.call(k_increaseAchievement, json.dump());
}

void Self::unlockAchievement(const std::string& achievement_id) {
    nlohmann::json json;
    json[k_achievementId] = achievement_id;
    bridge_.call(k_unlockAchievement, json.dump());
}

void Self::showLeaderboard(const std::string& leaderboard_id) {
    nlohmann::json json;
    json[k_leaderboardId] = leaderboard_id;
    bridge_.call(k_showLeaderboard, json.dump());
}

void Self::showAllLeaderboards() {
    bridge_.call(k_showAllLeaderboards);
}

void Self::submitScore(const std::string& leaderboard_id, int64_t score) {
    nlohmann::json json;
    json[k_leaderboardId] = leaderboard_id;
    json[k_score] = score;
    bridge_.call(k_submitScore, json.dump());
}
} // namespace Google
} // namespace ee
