//
//  FacebookBannerAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#ifndef EE_X_FACEBOOK_BANNER_AD_HPP
#define EE_X_FACEBOOK_BANNER_AD_HPP

#include "ee/ads/AdViewInterface.hpp"

namespace ee {
namespace facebook {
class FacebookAds;

class BannerAd : public AdViewInterface {
private:
    using Self = BannerAd;
    using Super = AdViewInterface;

public:
    virtual ~BannerAd() override;

    /// @see Super.
    virtual bool isLoaded() const override;

    /// @see Super.
    virtual void load() override;

    /// @see Super.
    virtual std::pair<int, int> getPosition() const override;

    /// @see Super.
    virtual void setPosition(int x, int y) override;

    /// @see Super.
    virtual std::pair<int, int> getSize() const override;

    /// @see Super.
    virtual void setSize(int width, int height) override;

    /// @see Super.
    virtual void setVisible(bool visible) override;

protected:
    friend FacebookAds;

    explicit BannerAd(FacebookAds* plugin, const std::string& adId);

private:
    FacebookAds* plugin_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_BANNER_AD_HPP */
