//
//  Created by Kiet Le on 12/24/2019.
//
//

#include "ee/play/PlayBridge.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

namespace ee {
namespace play {
namespace {
// clang-format off
constexpr auto k__isSignedIn           = "Play_isSignedIn";
constexpr auto k_signin                = "Play_signin";
constexpr auto k_signout               = "Play_signout";
constexpr auto k_showAchievements      = "Play_showAchievements";
constexpr auto k_increaseAchievement   = "Play_increaseAchievement";
constexpr auto k_unlockAchievement     = "Play_unlockAchievement";
constexpr auto k_showLeaderboard       = "Play_showLeaderboard";
constexpr auto k_showAllLeaderboards   = "Play_showAllLeaderboards";
constexpr auto k_submitScore           = "Play_submitScore";

constexpr auto k_onSignedIn           = "Play_onSignedIn";
// clang-format on
} // namespace

namespace {
// clang-format off
constexpr auto k_silent_sign_in        = "silent_sign_in";
constexpr auto k_achievementId         = "achievement_id";
constexpr auto k_increment             = "increment";
constexpr auto k_leaderboardId         = "leaderboard_id";
constexpr auto k_score                 = "score";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge()
    : bridge_(MessageBridge::getInstance()) {
    bridge_.registerHandler(
        [this](const std::string& message) {
            onSignedIn(core::toBool(message));
            return "";
        },
        k_onSignedIn);
}

Self::~Bridge() = default;

void Self::destroy() {
    bridge_.deregisterHandler(k_onSignedIn);
}

bool Self::isSignedIn() {
    auto response = bridge_.call(k__isSignedIn);
    return core::toBool(response);
}

void Self::signin(bool silentSignIn) {
    nlohmann::json json;
    json[k_silent_sign_in] = silentSignIn;
    bridge_.call(k_signin, json.dump());
}

void Self::signout() {
    bridge_.call(k_signout);
}

void Self::setLoginCallback(const LoginResultCallback& callback) {
    callback_ = callback;
}

void Self::onSignedIn(bool success) {
    if (callback_) {
        callback_(success);
    }
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
} // namespace play
} // namespace ee
