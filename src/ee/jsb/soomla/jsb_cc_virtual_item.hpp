//
//  jsb_cc_virtual_item.hpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#ifndef JSB_CC_VIRTUAL_ITEM_HPP
#define JSB_CC_VIRTUAL_ITEM_HPP

namespace se {
class Object;
} // namespace se

namespace soomla {

extern se::Object* __jsb_CCVirtualItem_proto;
extern se::Class* __jsb_CCVirtualItem_class;

se::Object* getVirtualItemProto();

bool register_cc_virtual_item_manual(se::Object* object);

} // namespace soomla

#endif /* JSB_CC_VIRTUAL_ITEM_HPP */
