#ifndef EE_X_I_COST_CENTER_ANALYTICS_BRIDGE_HPP
#define EE_X_I_COST_CENTER_ANALYTICS_BRIDGE_HPP

#ifdef __cplusplus

#include <unordered_map>
#include <variant>

#include <ee/core/IPlugin.hpp>
#include <ee/core/ILibraryAnalytics.h>

#include "ee/cost_center_analytics/CostCenterAnalyticsFwd.hpp"

namespace ee::cost_center::analytics {
    class IBridge : public IPlugin,
                    public ILibraryAnalytics {
    public:
        [[nodiscard]] virtual Task<bool> initialize() = 0;
    };
} // namespace

#endif // __cplusplus

#endif // EE_X_I_COST_CENTER_ANALYTICS_BRIDGE_HPP
