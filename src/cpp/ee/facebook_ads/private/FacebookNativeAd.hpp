//
//  FacebookNativeAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#ifndef EE_X_FACEBOOK_NATIVE_AD_HPP
#define EE_X_FACEBOOK_NATIVE_AD_HPP

#include <ee/ads/IBannerAd.hpp>
#include <ee/ads/internal/BannerAdHelper.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/facebook_ads/FacebookAdsFwd.hpp"

namespace ee {
namespace facebook_ads {
class NativeAd : public IBannerAd, public ObserverManager<AdObserver> {
public:
    explicit NativeAd(IMessageBridge& bridge, const Logger& logger,
                      Bridge* plugin, const std::string& adId);
    virtual ~NativeAd() override;

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
    void createInternalAd();
    void destroyInternalAd();

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onClicked();

    IMessageBridge& bridge_;
    const Logger& logger_;
    Bridge* plugin_;
    std::string adId_;
    ads::MessageHelper messageHelper_;
    ads::BannerAdHelper helper_;

    /// Whether the current internal ad is loaded.
    /// Facebook native ads can not be loaded twice.
    bool attempted_;

    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace facebook_ads
} // namespace ee

#endif /* EE_X_FACEBOOK_NATIVE_AD_HPP */
