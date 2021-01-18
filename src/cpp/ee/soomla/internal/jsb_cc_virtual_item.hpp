//
//  jsb_cc_virtual_item.hpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#ifndef EE_X_JSB_CC_VIRTUAL_ITEM_HPP
#define EE_X_JSB_CC_VIRTUAL_ITEM_HPP

#include <ee/soomla/SoomlaFwd.hpp>

namespace soomla {
se::Object* getCCVirtualItemProto();

bool register_cc_virtual_item_manual(se::Object* object);
} // namespace soomla

#endif /* EE_X_JSB_CC_VIRTUAL_ITEM_HPP */
