//
//  GenericAd.hpp
//  ee-x-d542b565
//
//  Created by eps on 1/7/21.
//

#ifndef EE_X_GENERIC_AD_HPP
#define EE_X_GENERIC_AD_HPP

#include <ee/ads/IFullScreenAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/services/ServicesFwd.hpp"

namespace ee {
namespace services {
class GenericAd : public IAd, public ObserverManager<AdObserver> {
public:
    explicit GenericAd(const std::shared_ptr<IFullScreenAd>& ad, int interval);
    virtual ~GenericAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    [[nodiscard]] virtual Task<bool> load() override;

    [[nodiscard]] Task<AdResult> show();

private:
    static Task<bool> testConnection(float timeOut);

    void updateCapping();

    std::shared_ptr<IFullScreenAd> ad_;
    bool capped_;
    int interval_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace services
} // namespace ee

#endif /* EE_X_GENERIC_AD_HPP */
