//
//  FirebasePerformance.hpp
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/8/19.
//

#ifndef EE_X_FIREBASE_PERFORMANCE_BRIDGE_HPP
#define EE_X_FIREBASE_PERFORMANCE_BRIDGE_HPP

#include "ee/firebase_performance/IFirebasePerformanceBridge.hpp"

namespace ee {
namespace firebase {
namespace performance {
class Bridge final : public IBridge {
public:
    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual Task<bool> initialize() override;
    virtual bool isDataCollectionEnabled() override;
    virtual void setDataCollectionEnabled(bool enabled) override;
    virtual std::shared_ptr<ITrace> newTrace(const std::string& name) override;

private:
    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
};
} // namespace performance
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_PERFORMANCE_BRIDGE_HPP */
