//
//  CocosAdView.hpp
//  Pods
//
//  Created by eps on 6/18/20.
//

#ifndef EE_X_GENERIC_BANNER_AD_HPP
#define EE_X_GENERIC_BANNER_AD_HPP

#include <ee/ads/IBannerAd.hpp>

#include "ee/services/internal/GenericAd.hpp"

namespace ee {
namespace services {
class GenericBannerAd : public IBannerAd, public GenericAd {
public:
    explicit GenericBannerAd(const std::shared_ptr<IBannerAd>& ad,
                             const std::shared_ptr<ads::ICapper>& loadCapper,
                             const std::shared_ptr<ads::IRetrier>& loadRetrier);
    virtual ~GenericBannerAd() override;

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
    std::unique_ptr<Metrics> metrics_;
    float sceneHeight_;
};
} // namespace services
} // namespace ee

#endif /* EE_X_GENERIC_BANNER_AD_HPP */
