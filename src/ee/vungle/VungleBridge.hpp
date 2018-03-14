#ifndef EE_X_VUNGLE_BRIDGE_HPP
#define EE_X_VUNGLE_BRIDGE_HPP

#include "ee/VungleFwd.hpp"
#include "ee/ads/IRewardedVideo.hpp"

#include <map>

namespace ee {
namespace vungle {
class Vungle final {
public:
    Vungle();
    ~Vungle();

    /// Initializes Vungle with the specified game ID.
    void initialize(const std::string& gameId, const std::string& placementId);

    /// Creates a rewarded video.
    std::shared_ptr<IRewardedVideo> createRewardedVideo(const std::string& placementId);

private:
    friend RewardedVideo;

    bool destroyRewardedVideo(const std::string& placementId);

    bool hasRewardedVideo(const std::string& placementId) const;
    bool showRewardedVideo(const std::string& placementId);

    void onStart();
    void onEnd(bool wasSuccessfulView);
    void onUnavailable();

    bool errored_;
//    RewardedVideo* rewardedVideo_;
    std::map<std::string, RewardedVideo*> rewardedVideos_;
};
} // namespace vungle
} // namespace ee

#endif /* EE_X_VUNGLE_BRIDGE_HPP */
