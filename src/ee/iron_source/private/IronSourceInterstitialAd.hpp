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
namespace iron_source {
class InterstitialAd : public IInterstitialAd {
public:
    virtual ~InterstitialAd() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit InterstitialAd(const Logger& logger, Bridge* plugin,
                            const std::string& placementId);

private:
    friend Bridge;

    const Logger& logger_;
    Bridge* plugin_;
    std::string placementId_;
};
} // namespace iron_source
} // namespace ee

#endif /* EE_X_IRON_SOURCE_INTERSTITIAL_AD_HPP */
