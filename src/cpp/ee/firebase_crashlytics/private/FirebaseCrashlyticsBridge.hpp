#ifndef EE_X_FIREBASE_CRASHLYTICS_BRIDGE_HPP
#define EE_X_FIREBASE_CRASHLYTICS_BRIDGE_HPP

#include "ee/firebase_crashlytics/IFirebaseCrashlyticsBridge.hpp"

namespace ee {
namespace firebase {
namespace crashlytics {
class Bridge final : public IBridge {
public:
    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual void log(const std::string& message) override;

private:
    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
};
} // namespace crashlytics
} // namespace firebase
} // namespace ee

#endif // EE_X_FIREBASE_CRASHLYTICS_BRIDGE_HPP
