#ifndef EE_X_AD_COLONY_BRIDGE_HPP
#define EE_X_AD_COLONY_BRIDGE_HPP

#include "ee/ad_colony/IAdColonyBridge.hpp"

namespace ee {
namespace ad_colony {
class Bridge final : public IBridge {
public:
    explicit Bridge(IMessageBridge& bridge);
    virtual ~Bridge() override;

    virtual void destroy() override;
    virtual Task<bool>
    initialize(const std::string& appId,
               const std::vector<std::string>& zoneIds) override;

private:
    IMessageBridge& bridge_;
    const Logger& logger_;
};
} // namespace ad_colony
} // namespace ee

#endif // EE_X_AD_COLONY_BRIDGE_HPP