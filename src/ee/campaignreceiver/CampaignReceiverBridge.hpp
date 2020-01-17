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
namespace campaignreceiver {

using OnReceivedLinkCallback = std::function<void(std::string)>;
class CampaignReceiver final {
public:
    CampaignReceiver();
    ~CampaignReceiver();

    explicit CampaignReceiver(const Logger& logger);

    void initialize(const OnReceivedLinkCallback& callback);

private:
    IMessageBridge& bridge_;
    const Logger& logger_;
    OnReceivedLinkCallback callback_;
};
} // namespace campaignreceiver
} // namespace ee

#endif /* EE_X_CAMPAIGN_RECEIVER_BRIDGE_HPP */
