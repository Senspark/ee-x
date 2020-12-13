//
//  IVungleBridge.hpp
//  ee-x-a41c1ed3
//
//  Created by eps on 8/20/20.
//

#ifndef EE_X_I_VUNGLE_BRIDGE_HPP
#define EE_X_I_VUNGLE_BRIDGE_HPP

#ifdef __cplusplus

#include <ee/core/IPlugin.hpp>

#include "ee/vungle/VungleFwd.hpp"

namespace ee {
namespace vungle {
class IBridge : public IPlugin {
public:
    /// Initializes Vungle with the specified game ID.
    [[nodiscard]] virtual Task<bool> initialize(const std::string& appId) = 0;

    /// Creates a rewarded video.
    virtual std::shared_ptr<IRewardedAd>
    createRewardedAd(const std::string& adId) = 0;
};
} // namespace vungle
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_VUNGLE_BRIDGE_HPP */
