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
#include "ee/ads/internal/AdViewHelper.hpp"

namespace ee {
namespace facebook {
class FacebookAds;

class NativeAd : public AdViewInterface {
private:
    using Self = NativeAd;
    using Super = AdViewInterface;

public:
    virtual ~NativeAd() override;

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

    explicit NativeAd(FacebookAds* plugin, const std::string& adId);

private:
    FacebookAds* plugin_;
    ads::AdViewHelper helper_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_NATIVE_AD_HPP */
