#ifndef EE_X_VUNGLE_BRIDGE_HPP
#define EE_X_VUNGLE_BRIDGE_HPP

#include "ee/ads/RewardedVideoInterface.hpp"

namespace ee {
namespace vungle {
class RewardedVideo;

class Vungle final {
public:
    Vungle();
    ~Vungle();

    void initialize(const std::string& gameId);

    std::shared_ptr<RewardedVideoInterface> createRewardedVideo();

private:
    friend RewardedVideo;

    bool destroyRewardedVideo();

    bool hasRewardedVideo() const;
    bool showRewardedVideo();

    void onStart();
    void onEnd(bool wasSuccessfulView);
    void onUnavailable();

    bool errored_;
    RewardedVideo* rewardedVideo_;
};
} // namespace vungle
} // namespace ee

#endif /* EE_X_VUNGLE_BRIDGE_HPP */
