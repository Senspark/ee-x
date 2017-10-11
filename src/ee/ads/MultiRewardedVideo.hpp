//
//  RewardedVideoList.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_MULTI_REWARDED_VIDEO_HPP
#define EE_X_MULTI_REWARDED_VIDEO_HPP

#include <memory>
#include <vector>

#include "ee/ads/RewardedVideoInterface.hpp"

namespace ee {
namespace ads {
class MultiRewardedVideo : public RewardedVideoInterface {
private:
    using Self = MultiRewardedVideo;
    using Super = RewardedVideoInterface;

public:
    MultiRewardedVideo();
    virtual ~MultiRewardedVideo() override;

    Self& addItem(const std::shared_ptr<RewardedVideoInterface>& item);

    /// @see Super.
    virtual bool isLoaded() const override;

    /// @see Super.
    virtual void load() override;

    /// @see Super.
    virtual bool show() override;

private:
    std::vector<std::shared_ptr<RewardedVideoInterface>> items_;
};
} // namespace ads

using ads::MultiRewardedVideo;
} // namespace ee

#endif /* EE_X_MULTI_REWARDED_VIDEO_HPP */
