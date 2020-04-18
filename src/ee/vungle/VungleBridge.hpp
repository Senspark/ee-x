#ifndef EE_X_VUNGLE_BRIDGE_HPP
#define EE_X_VUNGLE_BRIDGE_HPP

#include <map>

#include "ee/VungleFwd.hpp"

namespace ee {
namespace vungle {
class Bridge final {
public:
    Bridge();
    ~Bridge();

    explicit Bridge(const Logger& logger);

    /// Initializes Vungle with the specified game ID.
    void initialize(const std::string& gameId);

    [[deprecated]] void initialize(const std::string& gameId,
                                   const std::string& adId);

    /// Creates a rewarded video.
    std::shared_ptr<IRewardedAd> createRewardedAd(const std::string& adId);

private:
    friend RewardedAd;

    bool destroyRewardedAd(const std::string& adId);

    bool hasRewardedAd(const std::string& adId) const;
    void loadRewardedAd(const std::string& adId) const;
    void showRewardedAd(const std::string& adId);

    void onLoaded(const std::string& adId);
    void onFailedToLoad(const std::string& adId, const std::string& message);
    void onFailedToShow(const std::string& adId, const std::string& message);
    void onClosed(const std::string& adId, bool rewarded);

    void onMediationAdClosed(const std::string& adId, bool rewarded);

    IMessageBridge& bridge_;
    const Logger& logger_;

    std::map<std::string, std::shared_ptr<RewardedAd>> rewardedAds_;

    std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>> rewardedAdDisplayer_;
};
} // namespace vungle
} // namespace ee

#endif /* EE_X_VUNGLE_BRIDGE_HPP */
