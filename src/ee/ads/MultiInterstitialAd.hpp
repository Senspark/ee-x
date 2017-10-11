//
//  MultiInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_MULTI_INTERSTITIAL_AD_HPP
#define EE_X_MULTI_INTERSTITIAL_AD_HPP

#include <memory>
#include <vector>

#include "ee/ads/InterstitialAdInterface.hpp"

namespace ee {
namespace ads {
class MultiInterstitialAd : public InterstitialAdInterface {
private:
    using Self = MultiInterstitialAd;
    using Super = InterstitialAdInterface;

public:
    MultiInterstitialAd();
    virtual ~MultiInterstitialAd() override;

    Self& addItem(const std::shared_ptr<InterstitialAdInterface>& item);

    /// @see Super.
    virtual bool isLoaded() const override;

    /// @see Super.
    virtual void load() override;

    /// @see Super.
    virtual bool show() override;

private:
    std::vector<std::shared_ptr<InterstitialAdInterface>> items_;
};
} // namespace ads

using ads::MultiInterstitialAd;
} // namespace ee

#endif /* EE_X_MULTI_INTERSTITIAL_AD_HPP */
