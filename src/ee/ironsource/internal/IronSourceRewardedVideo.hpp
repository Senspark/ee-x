//
//  IronSourceRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_IRON_SOURCE_REWARDED_VIDEO_HPP
#define EE_X_IRON_SOURCE_REWARDED_VIDEO_HPP

#include "ee/IronSourceFwd.hpp"
#include "ee/ads/IRewardedVideo.hpp"

namespace ee {
namespace ironsource {
class RewardedVideo : public IRewardedVideo {
private:
    using Super = IRewardedVideo;

public:
    virtual ~RewardedVideo() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit RewardedVideo(const Logger& logger, IronSource* plugin,
                           const std::string& placementId);

private:
    friend IronSource;

    const Logger& logger_;
    IronSource* plugin_;
    std::string placementId_;
};
} // namespace ironsource
} // namespace ee

#endif /* EE_X_IRON_SOURCE_REWARDED_VIDEO_HPP */
