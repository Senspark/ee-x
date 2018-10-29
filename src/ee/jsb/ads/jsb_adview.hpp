//
//  jsb_adview.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#ifndef JSB_ADVIEW_HPP__
#define JSB_ADVIEW_HPP__

#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace ads {

se::Class* getIAdViewClass();

bool register_adview_manual(se::Object* object);
} // namespace ads
} // namespace ee

#endif /* JSB_ADVIEW_HPP__ */
