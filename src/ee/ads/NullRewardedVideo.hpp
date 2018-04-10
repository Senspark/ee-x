//
//  NullRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/27/17.
//
//

#ifndef EE_X_NULL_REWARDED_VIDEO_HPP
#define EE_X_NULL_REWARDED_VIDEO_HPP

#include "ee/ads/IRewardedVideo.hpp"

namespace ee {
namespace ads {
class NullRewardedVideo : public IRewardedVideo {
public:
    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_NULL_REWARDED_VIDEO_HPP */
