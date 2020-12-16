//
//  jsb_interstitial_ad.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#ifndef EE_X_JSB_I_INTERSTITIAL_AD_HPP
#define EE_X_JSB_I_INTERSTITIAL_AD_HPP

#include <ee/jsb/JsbCoreFwd.hpp>

namespace ee {
namespace ads {
se::Class* getIInterstitialClass();

bool registerJsbIInterstitialAd(se::Object* global);
} // namespace ads
} // namespace ee

#endif /* EE_X_JSB_I_INTERSTITIAL_AD_HPP */
