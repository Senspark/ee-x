//
//  IronSourceInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_IRON_SOURCE_INTERSTITIAL_AD_HPP
#define EE_X_IRON_SOURCE_INTERSTITIAL_AD_HPP

#include <string>

#include <ee/ads/IInterstitialAd.hpp>

#include "ee/IronSourceFwd.hpp"

namespace ee {
namespace ironsource {
class InterstitialAd : public IInterstitialAd {
public:
    virtual ~InterstitialAd() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit InterstitialAd(const Logger& logger, IronSource* plugin,
                            const std::string& placementId);

private:
    friend IronSource;

    const Logger& logger_;
    IronSource* plugin_;
    std::string placementId_;
};
} // namespace ironsource
} // namespace ee

#endif /* EE_X_IRON_SOURCE_INTERSTITIAL_AD_HPP */
