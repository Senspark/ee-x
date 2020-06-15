//
//  CampaignReceiver.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_CAMPAIGN_RECEIVER_BRIDGE_HPP
#define EE_X_CAMPAIGN_RECEIVER_BRIDGE_HPP

#ifdef __cplusplus

#include <functional>

#include <ee/core/IPlugin.hpp>

#include "ee/CampaignReceiverFwd.hpp"

namespace ee {
namespace campaign_receiver {
using OnReceivedLinkCallback = std::function<void(const std::string&)>;

class Bridge final : public IPlugin {
public:
    Bridge();
    ~Bridge();

    explicit Bridge(const Logger& logger);

    virtual void destroy() override;

    void initialize(const OnReceivedLinkCallback& callback);

private:
    IMessageBridge& bridge_;
    const Logger& logger_;
    OnReceivedLinkCallback callback_;
};
} // namespace campaign_receiver
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_CAMPAIGN_RECEIVER_BRIDGE_HPP */
