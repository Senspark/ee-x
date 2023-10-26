//
//  FirebaseAnalytics.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_FIREBASE_ANALYTICS_BRIDGE_HPP
#define EE_X_FIREBASE_ANALYTICS_BRIDGE_HPP

#include <functional>
#include "ee/firebase_analytics/IFirebaseAnalyticsBridge.hpp"

namespace ee {
namespace firebase {
namespace analytics {
class Bridge : public IBridge {
public:
    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual Task<bool> initialize() override;
    virtual void setUserProperty(const std::string& key,
                                 const std::string& value) override;
    virtual void trackScreen(const std::string& name) override;
    virtual void
    logEvent(const std::string& name,
             const std::unordered_map<
                 std::string, std::variant<std::int64_t, double, std::string>>&
                 parameters) override;
    
    void logRevenue(const AdRevenue &adRevenue) override;
    void logRevenue(const IapRevenue &iapRevenue) override;

    void pushGameLevel(int levelNo, const std::string &levelMode) override {}
    void popGameLevel(bool winGame) override {}

private:
    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
};
} // namespace analytics
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_ANALYTICS_BRIDGE_HPP */
