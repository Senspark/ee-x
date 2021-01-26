#ifndef EE_X_IRON_SOURCE_BANNER_AD_HPP
#define EE_X_IRON_SOURCE_BANNER_AD_HPP

#include <ee/ads/IBannerAd.hpp>
#include <ee/ads/internal/BannerAdHelper.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/iron_source/IronSourceFwd.hpp"

namespace ee {
namespace iron_source {
class BannerAd final : public IBannerAd, public ObserverManager<AdObserver> {
public:
    explicit BannerAd(IMessageBridge& bridge, const Logger& logger,
                      Bridge* plugin, const std::string& adId,
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
    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onClicked();

    IMessageBridge& bridge_;
    const Logger& logger_;
    Bridge* plugin_;
    std::string adId_;
    ads::MessageHelper messageHelper_;
    ads::BannerAdHelper helper_;

    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace iron_source
} // namespace ee

#endif // EE_X_IRON_SOURCE_BANNER_AD_HPP