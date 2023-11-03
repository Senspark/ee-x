//
// Created by Nhan on 05/07/2023.
//

#ifndef EE_X_APP_LOVIN_MAX_BANNER_AD_HPP
#define EE_X_APP_LOVIN_MAX_BANNER_AD_HPP

#include <string>
#include <memory>

#include "ee/app_lovin_max/AppLovinMaxFwd.hpp"
#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IBannerAd.hpp"
#include "ee/ads/internal/BannerAdHelper.hpp"
#include "ee/ads/internal/MessageHelper.hpp"

namespace ee {
namespace app_lovin_max {
class BannerAd : public IBannerAd,
                 public ObserverManager<AdObserver> {
public:
    using Destroyer = std::function<void()>;

    explicit BannerAd(const std::string& prefix, IMessageBridge& bridge,
                             ILogger& logger, const Destroyer& destroyer,
                             const std::string& network,
                             const std::string& adId,
                             const std::pair<int, int>& size);
    virtual ~BannerAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;

    virtual std::pair<float, float> getAnchor() const override;
    virtual void setAnchor(float x, float y) override;

    virtual std::pair<float, float> getPosition() const override;
    virtual void setPosition(float x, float y) override;

    virtual std::pair<float, float> getSize() const override;
    virtual void setSize(float width, float height) override;

    virtual bool isVisible() const override;
    virtual void setVisible(bool visible) override;

private:
    friend Bridge;

    void onLoaded();
    void onFailedToLoad(int code, const std::string& message);
    void onClicked();
    void onAdPaid(const ads::AdPaidResult& result);

    bool isVisible_;
    std::string prefix_;
    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
    std::string network_;
    std::string adId_;
    ads::MessageHelper messageHelper_;
    ads::BannerAdHelper helper_;
    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace app_lovin_max
} // namespace ee

#endif //EE_X_APP_LOVIN_MAX_BANNER_AD_HPP
