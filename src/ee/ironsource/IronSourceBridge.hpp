#ifndef EE_X_IRON_SOURCE_BRIDGE_HPP
#define EE_X_IRON_SOURCE_BRIDGE_HPP

#include "ee/ads/RewardedVideoInterface.hpp"

namespace ee {
namespace ironsource {
class IronSource final : public RewardedVideoInterface {
public:
    IronSource();
    virtual ~IronSource() override;

    void initialize(const std::string& gameId);

    /// @see Super.
    virtual bool showRewardedVideo(const std::string& placementId) override;
};
} // namespace ironsource
} // namespace ee

#endif /* EE_X_IRON_SOURCE_BRIDGE_HPP */
