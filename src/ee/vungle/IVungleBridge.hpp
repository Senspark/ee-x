//
//  IVungleBridge.hpp
//  ee-x-a41c1ed3
//
//  Created by eps on 8/20/20.
//

#ifndef EE_X_I_VUNGLE_BRIDGE_HPP
#define EE_X_I_VUNGLE_BRIDGE_HPP

#ifdef __OBJC__
/// https://support.vungle.com/hc/en-us/articles/360002925791-Integrate-Vungle-SDK-for-iOS
/// Step 3: Swift Only: Create a Bridging Header File.
#import <VungleSDK/VungleSDK.h>
#endif // __OBJC__

#ifdef __cplusplus

#include <ee/core/IPlugin.hpp>

#include "ee/VungleFwd.hpp"

namespace ee {
namespace vungle {
class IBridge : public IPlugin {
public:
    /// Initializes Vungle with the specified game ID.
    virtual void initialize(const std::string& appId) = 0;

    /// Creates a rewarded video.
    virtual std::shared_ptr<IRewardedAd>
    createRewardedAd(const std::string& adId) = 0;
};
} // namespace vungle
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_VUNGLE_BRIDGE_HPP */
