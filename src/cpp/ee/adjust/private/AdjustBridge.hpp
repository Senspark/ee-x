//
//  AdjustBridge.hpp
//  Adjust
//
//  Created by eps on 8/19/20.
//

#ifndef EE_X_ADJUST_BRIDGE_HPP
#define EE_X_ADJUST_BRIDGE_HPP

#include "ee/adjust/IAdjustBridge.hpp"

namespace ee {
namespace adjust {
class Bridge final : public IBridge {
public:
    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual void initialize(const AdjustConfig& config) override;
    virtual void setEnabled(bool enabled) override;
    virtual Task<std::string> getAdvertisingIdentifier() const override;
    virtual std::string getDeviceIdentifier() const override;
    virtual void setPushToken(const std::string& token) override;
    virtual void trackEvent(const std::string& token) override;

private:
    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
};
} // namespace adjust
} // namespace ee

#endif /* EE_X_ADJUST_BRIDGE_HPP */
