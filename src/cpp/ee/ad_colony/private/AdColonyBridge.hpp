#ifndef EE_X_AD_COLONY_BRIDGE_HPP
#define EE_X_AD_COLONY_BRIDGE_HPP

#include "ee/ad_colony/IAdColonyBridge.hpp"

namespace ee {
namespace ad_colony {
class Bridge final : public IBridge {
public:
    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual Task<bool>
    initialize(const std::string& appId,
               const std::vector<std::string>& zoneIds) override;

private:
    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
};
} // namespace ad_colony
} // namespace ee

#endif // EE_X_AD_COLONY_BRIDGE_HPP