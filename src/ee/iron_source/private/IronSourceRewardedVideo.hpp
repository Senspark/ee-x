//
//  IronSourceRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_IRON_SOURCE_REWARDED_VIDEO_HPP
#define EE_X_IRON_SOURCE_REWARDED_VIDEO_HPP

#include <string>

#include <ee/ads/IRewardedVideo.hpp>

#include "ee/IronSourceFwd.hpp"

namespace ee {
namespace iron_source {
class RewardedVideo : public IRewardedVideo {
private:
    using Super = IRewardedVideo;

public:
    virtual ~RewardedVideo() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit RewardedVideo(const Logger& logger, Bridge* plugin,
                           const std::string& placementId);

private:
    friend Bridge;

    const Logger& logger_;
    Bridge* plugin_;
    std::string placementId_;
};
} // namespace iron_source
} // namespace ee

#endif /* EE_X_IRON_SOURCE_REWARDED_VIDEO_HPP */
