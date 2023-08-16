//
//  Created by Kiet Le on 12/24/2019.
//
//

#include "ee/play/private/PlayBridge.hpp"
#include "ee/play/private/CloudData.h"
#include "ee/play/private/InternalCloudDataHandler.h"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/Logger.hpp>
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
const auto kPushToCloud          = kPrefix + "PushToCloud";
const auto kPullFromCloud        = kPrefix + "PullFromCloud";
const auto kDeleteCloud          = kPrefix + "DeleteCloud";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge, ILogger& logger,
             const Destroyer& destroyer)
    : bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer) {}

Self::~Bridge() = default;

void Self::destroy() {
    destroyer_();
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

Task<bool> Self::pushToCloud(std::shared_ptr<ICloudDataHandler> handler, std::string title) {
    nlohmann::json json;
    json["title"] = title;
    json["data"] = handler->exportData();
    auto response = co_await bridge_.callAsync(kPushToCloud, json.dump());
    co_return core::toBool(response);
}

Task<std::shared_ptr<ICloudDataHandler>> Self::pullFromCloud() {
    auto response = co_await bridge_.callAsync(kPullFromCloud);
    auto json = nlohmann::json::parse(response);
    auto cloudData = CloudData::parseJson(json.dump());
    auto handler = std::shared_ptr<InternalCloudDataHandler>();
    handler->importData(cloudData->originData);
    co_return handler;
}

Task<bool> Self::deleteCloud() {
    auto response = co_await
    bridge_.callAsync(kDeleteCloud);
    co_return core::toBool(response);
}

} // namespace play
} // namespace ee
