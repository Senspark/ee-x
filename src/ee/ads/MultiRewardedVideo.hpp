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

#include "ee/ads/IRewardedVideo.hpp"

namespace ee {
namespace ads {
class MultiRewardedVideo : public IRewardedVideo {
private:
    using Self = MultiRewardedVideo;
    using Super = IRewardedVideo;

public:
    MultiRewardedVideo();
    explicit MultiRewardedVideo(Logger& logger);
    
    virtual ~MultiRewardedVideo() override;

    Self& addItem(const std::shared_ptr<IRewardedVideo>& item);

    /// @see Super.
    virtual bool isLoaded() const override;

    /// @see Super.
    virtual void load() override;

    /// @see Super.
    virtual bool show() override;

private:
    void assignCallbacks();
    void clearCallbacks();

    std::vector<std::shared_ptr<IRewardedVideo>> items_;
};
} // namespace ads

using ads::MultiRewardedVideo;
} // namespace ee

#endif /* EE_X_MULTI_REWARDED_VIDEO_HPP */
