//
//  jsb_cc_purchasable_virtual_item.hpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/18/18.
//

#ifndef jsb_cc_purchasable_virtual_item_hpp
#define jsb_cc_purchasable_virtual_item_hpp

#include "ee/jsb/jsb_fwd.hpp"

namespace soomla {

se::Object* getCCPurchasableVirtualItemProto();

bool register_cc_purchasable_virtual_item_manual(se::Object* object);

} // namespace soomla

#endif /* jsb_cc_purchasable_virtual_item_hpp */
