//
//  jsb_admob.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#ifndef JSB_ADMOB_HPP__
#define JSB_ADMOB_HPP__

#ifdef __cplusplus
#include "ee/jsb/admob/jsb_admob_bridge.hpp"
#include "ee/jsb/admob/jsb_native_ad_layout.hpp"
namespace ee {
namespace admob {
bool register_all_admob_manual(se::Object* object) {
    register_admob_bridge_manual(object);
    register_native_ad_layout_manual(object);
    return true;
}
} // namespace admob
} // namespace ee

#endif // __cplusplus
#endif // JSB_CORE_HPP
