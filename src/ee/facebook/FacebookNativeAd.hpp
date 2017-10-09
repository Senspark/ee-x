//
//  FacebookNativeAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#ifndef EE_X_FACEBOOK_NATIVE_AD_HPP
#define EE_X_FACEBOOK_NATIVE_AD_HPP

#include "ee/ads/AdViewInterface.hpp"

namespace ee {
namespace facebook {
class FacebookAds;

class FacebookNativeAd : public AdViewInterface {
private:
    using Self = FacebookNativeAd;
    using Super = AdViewInterface;

public:
    virtual ~FacebookNativeAd() override;

    /// @see Super.
    virtual bool isLoaded() const override;

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

    explicit FacebookNativeAd(FacebookAds* plugin, const std::string& adId);

private:
    FacebookAds* plugin_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_NATIVE_AD_HPP */
