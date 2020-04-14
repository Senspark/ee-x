//
//  FacebookNativeAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#ifndef EE_X_FACEBOOK_NATIVE_AD_HPP
#define EE_X_FACEBOOK_NATIVE_AD_HPP

#include <ee/ads/IAdView.hpp>
#include <ee/ads/internal/AdViewHelper.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/FacebookAdsFwd.hpp"

namespace ee {
namespace facebook_ads {
class NativeAd : public IAdView, public ObserverManager<IAdViewObserver> {
private:
    using Self = NativeAd;
    using Super = IAdView;

public:
    virtual ~NativeAd() override;

    virtual bool isLoaded() const override;
    virtual void load() override;

    virtual std::pair<float, float> getAnchor() const override;
    virtual void setAnchor(float x, float y) override;

    virtual std::pair<int, int> getPosition() const override;
    virtual void setPosition(int x, int y) override;

    virtual std::pair<int, int> getSize() const override;
    virtual void setSize(int width, int height) override;

    virtual bool isVisible() const override;
    virtual void setVisible(bool visible) override;

protected:
    friend Bridge;

    explicit NativeAd(IMessageBridge& bridge, const Logger& logger,
                      Bridge* plugin, const std::string& adId);

    bool createInternalAd();
    bool destroyInternalAd();

private:
    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onClicked();

    /// Whether the current internal ad is loaded.
    /// Facebook native ads can not be loaded twice.
    bool attempted_;

    /// Whether the ad is loading.
    bool loading_;

    std::string adId_;
    IMessageBridge& bridge_;
    const Logger& logger_;
    Bridge* plugin_;
    ads::AdViewHelper helper_;
};
} // namespace facebook_ads
} // namespace ee

#endif /* EE_X_FACEBOOK_NATIVE_AD_HPP */
