//
//  FirebaseAnalytics.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_COST_CENTER_ANALYTICS_BRIDGE_HPP
#define EE_X_COST_CENTER_ANALYTICS_BRIDGE_HPP

#include <functional>
#include "ee/cost_center_analytics/ICostCenterAnalyticsBridge.hpp"

namespace ee::cost_center::analytics {
    class Bridge : public IBridge {
    public:
        using Destroyer = std::function<void()>;

        explicit Bridge(IMessageBridge &bridge, ILogger &logger,
                        const Destroyer &destroyer);

        virtual ~Bridge() override;

        virtual void destroy() override;

        virtual Task<bool> initialize() override;

        void logRevenue(const AdRevenue &adRevenue) override;

        void logRevenue(const IapRevenue &iapRevenue) override;

    private:
        IMessageBridge &bridge_;
        ILogger &logger_;
        Destroyer destroyer_;
    };
} // namespace

#endif /* EE_X_COST_CENTER_ANALYTICS_BRIDGE_HPP */
