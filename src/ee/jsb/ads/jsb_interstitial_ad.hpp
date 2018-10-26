//
//  jsb_interstitial_ad.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#ifndef JSB_INTERSTITIAL_AD_HPP__
#define JSB_INTERSTITIAL_AD_HPP__

#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace ads {
se::Class* getIInterstitialClass();

bool register_interstitial_ad_manual(se::Object* object);
}
} // namespace ee

#endif /* jsb_interstitial_ad_hpp */
