//
//  IIronSourceBridge.hpp
//  ee-x-a41c1ed3
//
//  Created by eps on 8/20/20.
//

#ifndef EE_X_I_IRON_SOURCE_BRIDGE_HPP
#define EE_X_I_IRON_SOURCE_BRIDGE_HPP

#ifdef __OBJC__
#import <IronSource/IronSource.h>
#endif // __OBJC__

#ifdef __cplusplus

#include <memory>
#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/IronSourceFwd.hpp"

namespace ee {
namespace iron_source {
class IBridge : public IPlugin {
public:
    /// Initializes ironSource with the specified game ID.
    virtual void initialize(const std::string& appKey) = 0;

    /// Creates an interstitial ad with the specified placement ID.
    virtual std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& adId) = 0;

    /// Creates a rewarded vided with the specifie placement ID.
    virtual std::shared_ptr<IRewardedAd>
    createRewardedAd(const std::string& adId) = 0;
};
} // namespace iron_source
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_IRON_SOURCE_BRIDGE_HPP */
