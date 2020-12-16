//
//  jsb_adview.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#ifndef EE_X_JSB_I_AD_VIEW_HPP
#define EE_X_JSB_I_AD_VIEW_HPP

#include <ee/jsb/JsbCoreFwd.hpp>

namespace ee {
namespace ads {
se::Class* getIAdViewClass();

bool registerJsbIAdView(se::Object* global);
} // namespace ads
} // namespace ee

#endif /* EE_X_JSB_I_AD_VIEW_HPP */
