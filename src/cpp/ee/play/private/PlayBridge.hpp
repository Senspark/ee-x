//
//  Created by Kiet Le on 12/24/2019.
//
//

#ifndef EE_X_PLAY_BRIDGE_HPP
#define EE_X_PLAY_BRIDGE_HPP

#include "ee/play/IPlayBridge.hpp"

namespace ee {
namespace play {
class Bridge final : public IBridge {
public:
    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual bool isLoggedIn() const override;
    virtual Task<bool> logIn(bool silently = false) override;
    virtual Task<bool> logOut() override;
    virtual void showAchievements() override;
    virtual void incrementAchievement(const std::string& achievementId,
                                      double percent) override;
    virtual void unlockAchievement(const std::string& achievementId) override;
    virtual void showLeaderboard(const std::string& leaderboardId) override;
    virtual void showAllLeaderboards() override;
    virtual void submitScore(const std::string& leaderboardId,
                             std::int64_t score) override;

	// Cloud Save
	Task<bool> pushToCloud(std::shared_ptr<ICloudDataHandler> handler, std::string title) override;
	Task<std::shared_ptr<ICloudDataHandler>> pullFromCloud() override;
	Task<bool> deleteCloud() override;

private:
    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
};
} // namespace play
} // namespace ee

#endif /* EE_X_PLAY_BRIDGE_HPP */
