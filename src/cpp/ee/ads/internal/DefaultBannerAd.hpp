//
//  DefaultBannerAd.hpp
//  Pods
//
//  Created by eps on 1/26/21.
//

#ifndef EE_X_DEFAULT_BANNER_AD_HPP
#define EE_X_DEFAULT_BANNER_AD_HPP

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IBannerAd.hpp"
#include "ee/ads/internal/BannerAdHelper.hpp"
#include "ee/ads/internal/MessageHelper.hpp"

namespace ee {
namespace ads {
class DefaultBannerAd final : public IBannerAd,
                              public ObserverManager<AdObserver> {
public:
    using Destroyer = std::function<void()>;

    explicit DefaultBannerAd(const std::string& prefix, IMessageBridge& bridge,
                             const Logger& logger, const std::string& adId,
                             const Destroyer& destroyer,
                             const std::pair<int, int>& size);
    virtual ~DefaultBannerAd() override;

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
    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onClicked();

    std::string prefix_;
    IMessageBridge& bridge_;
    const Logger& logger_;
    Destroyer destroyer_;
    std::string adId_;
    MessageHelper messageHelper_;
    BannerAdHelper helper_;
    std::unique_ptr<IAsyncHelper<bool>> loader_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_DEFAULT_BANNER_AD_HPP */
