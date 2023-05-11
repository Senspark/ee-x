#ifndef EE_X_I_APP_LOVIN_MAX_BRIDGE_HPP
#define EE_X_I_APP_LOVIN_MAX_BRIDGE_HPP

#ifdef __cplusplus

#include <memory>
#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/app_lovin_max/AppLovinMaxFwd.hpp"

namespace ee {
namespace app_lovin_max {
class IBridge : public IPlugin {
public:
    [[nodiscard]] virtual Task<bool> initialize(const std::string& key) = 0;

    /// Disabled by default.
    virtual void setVerboseLogging(bool enabled) = 0;

    /// Disabled by default.
    virtual void setMuted(bool enabled) = 0;

    virtual std::shared_ptr<IFullScreenAd> createRewardedAd() = 0;
};
} // namespace app_lovin_max
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_APP_LOVIN_MAX_BRIDGE_HPP */
