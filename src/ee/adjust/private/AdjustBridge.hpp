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
    explicit Bridge(IMessageBridge& bridge);
    virtual ~Bridge() override;
    virtual void destroy() override;
    virtual void initialize(const AdjustConfig& config) override;
    virtual void setEnabled(bool enabled) override;
    virtual void trackEvent(const std::string& eventName) override;

private:
    IMessageBridge& bridge_;
};
} // namespace adjust
} // namespace ee

#endif /* EE_X_ADJUST_BRIDGE_HPP */
