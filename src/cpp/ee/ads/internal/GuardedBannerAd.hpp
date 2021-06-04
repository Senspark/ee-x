#ifndef EE_X_GUARDED_BANNER_AD_HPP
#define EE_X_GUARDED_BANNER_AD_HPP

#include "ee/ads/IBannerAd.hpp"
#include "ee/ads/internal/GuardedAd.hpp"

namespace ee {
namespace ads {
class GuardedBannerAd : public IBannerAd, public GuardedAd {
public:
    explicit GuardedBannerAd(const std::shared_ptr<IBannerAd>& ad);

    virtual ~GuardedBannerAd() override;

    virtual std::pair<float, float> getAnchor() const override;
    virtual void setAnchor(float x, float y) override;

    virtual std::pair<float, float> getPosition() const override;
    virtual void setPosition(float x, float y) override;

    virtual std::pair<float, float> getSize() const override;
    virtual void setSize(float width, float height) override;

    virtual bool isVisible() const override;
    virtual void setVisible(bool visible) override;

protected:
    virtual bool isDisplaying() const override;

private:
    std::shared_ptr<IBannerAd> ad_;
};
} // namespace ads
} // namespace ee

#endif // EE_X_GUARDED_BANNER_AD_HPP
