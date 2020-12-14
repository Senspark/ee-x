#ifndef EE_X_I_AD_COLONY_BRIDGE_HPP
#define EE_X_I_AD_COLONY_BRIDGE_HPP

#ifdef __cplusplus

#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/ad_colony/AdColonyFwd.hpp"

namespace ee {
namespace ad_colony {
class IBridge : public IPlugin {
public:
    [[nodiscard]] virtual Task<bool>
    initialize(const std::string& appId,
               const std::vector<std::string>& zoneIds) = 0;
};
} // namespace ad_colony
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_AD_COLONY_BRIDGE_HPP
