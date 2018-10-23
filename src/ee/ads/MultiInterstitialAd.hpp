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

#include "ee/ads/IInterstitialAd.hpp"

namespace ee {
namespace ads {
class MultiInterstitialAd : public IInterstitialAd {
private:
    using Self = MultiInterstitialAd;
    using Super = IInterstitialAd;

public:
    MultiInterstitialAd();
    virtual ~MultiInterstitialAd() override;

    Self& addItem(const std::shared_ptr<IInterstitialAd>& item);

    /// @see Super.
    virtual bool isLoaded() const override;

    /// @see Super.
    virtual void load() override;

    /// @see Super.
    virtual bool show() override;
    
    virtual void setOnClickedCallback(const OnClickedCallback& callback) override;

private:
    void assignCallbacks();
    void clearCallbacks();

    std::vector<std::shared_ptr<IInterstitialAd>> items_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_MULTI_INTERSTITIAL_AD_HPP */
