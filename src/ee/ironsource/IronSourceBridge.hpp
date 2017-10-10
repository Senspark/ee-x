#ifndef EE_X_IRON_SOURCE_BRIDGE_HPP
#define EE_X_IRON_SOURCE_BRIDGE_HPP

#include <map>

#include "ee/ads/RewardedVideoInterface.hpp"

namespace ee {
namespace ironsource {
class RewardedVideo;

class IronSource final {
public:
    IronSource();
    ~IronSource();

    void initialize(const std::string& gameId);

    std::shared_ptr<RewardedVideoInterface>
    createRewardedVideo(const std::string& placementId);

private:
    friend RewardedVideo;

    bool destroyRewardedVideo(const std::string& placementId);

    bool hasRewardedVideo() const;
    bool showRewardedVideo(const std::string& placementId);

    void onRewarded(const std::string& placementId);
    void onFailed();
    void onOpened();
    void onClosed();

    bool errored_;
    bool rewarded_;
    std::string placementId_;

    std::map<std::string, RewardedVideo*> rewardedVideos_;
};
} // namespace ironsource
} // namespace ee

#endif /* EE_X_IRON_SOURCE_BRIDGE_HPP */
