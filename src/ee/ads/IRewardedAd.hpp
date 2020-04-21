//
//  IRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/5/17.
//
//

#ifndef EE_X_I_REWARDED_AD_HPP
#define EE_X_I_REWARDED_AD_HPP

#include <functional>

#include <ee/CoroutineFwd.hpp>
#include <ee/core/IObserverManager.hpp>

#include "ee/AdsFwd.hpp"

namespace ee {
namespace ads {
enum class IRewardedAdResult {
    /// Failed to display the ad.
    Failed,

    /// Succeeded to display the ad and the user has canceled the ad.
    Canceled,

    /// Succeeded to display the ad and the user has completed the ad.
    Completed,
};

struct IRewardedAdObserver {
    /// Occurs when this ad is loaded.
    std::function<void()> onLoaded;

    /// Occurs when the user clicks this ad.
    std::function<void()> onClicked;
};

class IRewardedAd : public virtual IObserverManager<IRewardedAdObserver> {
public:
    virtual ~IRewardedAd() = default;

    /// Destroys this ad.
    virtual void destroy() = 0;

    /// Checks whether this ad is loaded.
    virtual bool isLoaded() const = 0;

    /// Attempts to load this ad.
    virtual Task<bool> load() = 0;

    /// Attempts to show this ad.
    virtual Task<IRewardedAdResult> show() = 0;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_I_REWARDED_AD_HPP */
