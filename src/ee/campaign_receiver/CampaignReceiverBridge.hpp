//
//  CampaignReceiver.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_CAMPAIGN_RECEIVER_BRIDGE_HPP
#define EE_X_CAMPAIGN_RECEIVER_BRIDGE_HPP

#include <functional>

#include "ee/CampaignReceiverFwd.hpp"

namespace ee {
namespace campaign_receiver {
using OnReceivedLinkCallback = std::function<void(const std::string&)>;

class Bridge final {
public:
    Bridge();
    ~Bridge();

    explicit Bridge(const Logger& logger);

    void destroy();

    void initialize(const OnReceivedLinkCallback& callback);

private:
    IMessageBridge& bridge_;
    const Logger& logger_;
    OnReceivedLinkCallback callback_;
};
} // namespace campaign_receiver
} // namespace ee

#endif /* EE_X_CAMPAIGN_RECEIVER_BRIDGE_HPP */
