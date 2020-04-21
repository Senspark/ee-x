//
//  AdMobNativeAd.hpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#ifndef EE_X_ADMOB_NATIVE_AD_HPP
#define EE_X_ADMOB_NATIVE_AD_HPP

#include <ee/ads/IAdView.hpp>
#include <ee/ads/internal/AdViewHelper.hpp>
#include <ee/core/SafeObserverManager.hpp>

#include "ee/AdMobFwd.hpp"

namespace ee {
namespace admob {
class NativeAd final : public IAdView,
                       public SafeObserverManager<IAdViewObserver> {
public:
    virtual ~NativeAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;

    virtual std::pair<float, float> getAnchor() const override;
    virtual void setAnchor(float x, float y) override;

    virtual std::pair<int, int> getPosition() const override;
    virtual void setPosition(int x, int y) override;

    virtual std::pair<int, int> getSize() const override;
    virtual void setSize(int width, int height) override;

    virtual bool isVisible() const override;
    virtual void setVisible(bool visible) override;

private:
    friend Bridge;

    explicit NativeAd(IMessageBridge& bridge, const Logger& logger,
                      Bridge* plugin, const std::string& adId);

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onClicked();

    IMessageBridge& bridge_;
    const Logger& logger_;
    Bridge* plugin_;
    std::string adId_;
    ads::MessageHelper messageHelper_;
    ads::AdViewHelper helper_;

    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace admob
} // namespace ee

#endif /* EE_X_ADMOB_NATIVE_AD_HPP */
