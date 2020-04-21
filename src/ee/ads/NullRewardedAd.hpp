//
//  NullRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/27/17.
//
//

#ifndef EE_X_NULL_REWARDED_AD_HPP
#define EE_X_NULL_REWARDED_AD_HPP

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IRewardedAd.hpp"

namespace ee {
namespace ads {
class NullRewardedAd : public IRewardedAd,
                       public ObserverManager<IRewardedAdObserver> {
public:
    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<IRewardedAdResult> show() override;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_NULL_REWARDED_AD_HPP */
