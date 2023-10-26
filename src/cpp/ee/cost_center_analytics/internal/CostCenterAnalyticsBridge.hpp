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

        void pushGameLevel(int levelNo, const std::string &levelMode) override;

        void popGameLevel(bool winGame) override;

    private:
        struct GameLevelData {
            int levelNo;
            std::string levelMode;
        };
        using Params = std::unordered_map<std::string, std::variant<std::int64_t, double, std::string>>;

    private:
        IMessageBridge &bridge_;
        ILogger &logger_;
        Destroyer destroyer_;
        std::unique_ptr<IapRevenue> lastIapRevenue_{nullptr};
        std::unique_ptr<GameLevelData> gameLevelData_{nullptr};

        void onPurchaseValidated(const std::string &jsonData);

        void logIapRevenue(const IapRevenue &iapRevenue, bool isTestPurchase);

        void addGameLevelDataToParams(Params& parameters);
    };
} // namespace

#endif /* EE_X_COST_CENTER_ANALYTICS_BRIDGE_HPP */
