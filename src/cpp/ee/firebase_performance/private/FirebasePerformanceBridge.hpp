//
//  FirebasePerformance.hpp
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/8/19.
//

#ifndef EE_X_FIREBASE_PERFORMANCE_BRIDGE_HPP
#define EE_X_FIREBASE_PERFORMANCE_BRIDGE_HPP

#ifdef __cplusplus

#include "ee/firebase_performance/IFirebasePerformanceBridge.hpp"

namespace ee {
namespace firebase {
namespace performance {
class Bridge final : public IBridge {
public:
    explicit Bridge(IMessageBridge& bridge);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual void setDataCollectionEnabled(bool enabled) = 0;
    virtual bool isDataCollectionEnabled() = 0;
    virtual std::shared_ptr<ITrace> newTrace(const std::string& name) = 0;

private:
    IMessageBridge& bridge_;
};
} // namespace performance
} // namespace firebase
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_FIREBASE_PERFORMANCE_BRIDGE_HPP */
