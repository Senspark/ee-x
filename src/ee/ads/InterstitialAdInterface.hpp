//
//  InterstitialAdInterface.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#ifndef EE_X_INTERSTITIAL_AD_INTERFACE_HPP
#define EE_X_INTERSTITIAL_AD_INTERFACE_HPP

#include <string>

namespace ee {
namespace ads {
class InterstitialAdInterface {
public:
    InterstitialAdInterface();
    virtual ~InterstitialAdInterface();

    virtual void cacheInterstitialAd(const std::string& adId) = 0;

    virtual bool showInterstitialAd() = 0;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_INTERSTITIAL_AD_INTERFACE_HPP */
