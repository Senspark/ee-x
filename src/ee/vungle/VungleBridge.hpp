#ifndef EE_X_VUNGLE_BRIDGE_HPP
#define EE_X_VUNGLE_BRIDGE_HPP

#include <vector>

#include "ee/ads/RewardedVideoInterface.hpp"

namespace ee {
namespace vungle {
class Vungle final : public RewardedVideoInterface {
public:
    static const std::string DefaultPlacementId;

    Vungle();
    virtual ~Vungle() override;

    void initialize(const std::string& gameId);

    /// @see Super.
    virtual bool
    isRewardedVideoReady(const std::string& placementId) const override;

    /// @see Super.
    virtual bool showRewardedVideo(const std::string& placementId) override;
};
} // namespace vungle
} // namespace ee

#endif /* EE_X_VUNGLE_BRIDGE_HPP */
