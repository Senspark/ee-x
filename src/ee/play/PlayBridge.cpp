//
//  Created by Kiet Le on 12/24/2019.
//
//

#include "ee/play/PlayBridge.hpp"

#include <ee/core/PluginManager.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace play {
namespace {
// clang-format off
const std::string kPrefix        = "PlayBridge";
const auto kIsLoggedIn           = kPrefix + "IsLoggedIn";
const auto kLogIn                = kPrefix + "LogIn";
const auto kLogOut               = kPrefix + "LogOut";
const auto kShowAchievements     = kPrefix + "ShowAchievements";
const auto kIncrementAchievement = kPrefix + "IncrementAchievement";
const auto kUnlockAchievement    = kPrefix + "UnlockAchievement";
const auto kShowLeaderboard      = kPrefix + "ShowLeaderboard";
const auto kShowAllLeaderboards  = kPrefix + "ShowAllLeaderboards";
const auto kSubmitScore          = kPrefix + "SubmitScore";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge()
    : bridge_(MessageBridge::getInstance()) {
    PluginManager::addPlugin(Plugin::Play);
}

Self::~Bridge() = default;

void Self::destroy() {
    PluginManager::removePlugin(Plugin::Play);
}

bool Self::isLoggedIn() const {
    auto response = bridge_.call(kIsLoggedIn);
    return core::toBool(response);
}

Task<bool> Self::logIn(bool silently) {
    nlohmann::json request;
    request["silently"] = silently;
    auto response = co_await bridge_.callAsync(kLogIn, request.dump());
    co_return core::toBool(response);
}

Task<bool> Self::logOut() {
    auto response = co_await bridge_.callAsync(kLogOut);
    co_return core::toBool(response);
}

void Self::showAchievements() {
    bridge_.call(kShowAchievements);
}

void Self::incrementAchievement(const std::string& achievementId,
                                double percent) {
    nlohmann::json json;
    json["achievement_id"] = achievementId;
    json["increment"] = percent;
    bridge_.call(kIncrementAchievement, json.dump());
}

void Self::unlockAchievement(const std::string& achievementId) {
    nlohmann::json json;
    json["achievement_id"] = achievementId;
    bridge_.call(kUnlockAchievement, json.dump());
}

void Self::showLeaderboard(const std::string& leaderboardId) {
    nlohmann::json json;
    json["leaderboard_id"] = leaderboardId;
    bridge_.call(kShowLeaderboard, json.dump());
}

void Self::showAllLeaderboards() {
    bridge_.call(kShowAllLeaderboards);
}

void Self::submitScore(const std::string& leaderboardId, std::int64_t score) {
    nlohmann::json json;
    json["leaderboard_id"] = leaderboardId;
    json["score"] = score;
    bridge_.call(kSubmitScore, json.dump());
}
} // namespace play
} // namespace ee
