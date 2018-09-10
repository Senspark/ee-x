//
//  jsb_interstitial_ad.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#ifndef JSB_INTERSTITIAL_AD_HPP__
#define JSB_INTERSTITIAL_AD_HPP__

#include "ee/jsb/core/jsb_templates.hpp"

#include <ee/AdsFwd.hpp>

namespace se {
class Object;
} // namespace se

namespace ee {
namespace ads {
bool register_interstitial_ad_manual(se::Object* object);
}

} // namespace ee

#endif /* jsb_interstitial_ad_hpp */
