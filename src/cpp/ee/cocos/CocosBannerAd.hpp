//
//  CocosAdView.hpp
//  Pods
//
//  Created by eps on 6/18/20.
//

#ifndef EE_X_COCOS_BANNER_AD_HPP
#define EE_X_COCOS_BANNER_AD_HPP

#ifdef __cplusplus

#include <ee/ads/IBannerAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/cocos/CocosFwd.hpp"

namespace ee {
namespace cocos {
class CocosBannerAd : public IBannerAd,
                      public ObserverManager<AdObserver> {
public:
    explicit CocosBannerAd(const std::shared_ptr<IBannerAd>& ad);

    virtual ~CocosBannerAd() override;

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
    std::shared_ptr<IBannerAd> ad_;
    std::unique_ptr<ObserverHandle> handle_;
    std::unique_ptr<Metrics> metrics_;
    float sceneHeight_;
};
} // namespace cocos
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_COCOS_BANNER_AD_HPP */
