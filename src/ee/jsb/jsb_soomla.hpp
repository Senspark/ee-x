//
//  jsb_soomla.hpp
//  ee-x
//
//  Created by Kiet Le on 9/12/18.
//

#ifndef JSB_SOOMLA_HPP__
#define JSB_SOOMLA_HPP__

#ifdef __cplusplus
#include "ee/jsb/soomla/jsb_cc_error.hpp"
#include "ee/jsb/soomla/jsb_cc_soomla.hpp"
#include "ee/jsb/soomla/jsb_cc_soomla_store.hpp"
#include "ee/jsb/soomla/jsb_cc_store_info.hpp"
#include "ee/jsb/soomla/jsb_cc_store_inventory.hpp"
#include "ee/jsb/soomla/jsb_cc_virtual_item.hpp"
#include "ee/jsb/soomla/jsb_cc_lifetime_vg_builder.hpp"
#include "ee/jsb/soomla/jsb_cc_purchase_with_market_builder.hpp"
#include "ee/jsb/soomla/jsb_cc_store_assets_builder.hpp"
#include "ee/jsb/soomla/jsb_cc_virtual_currency_pack_builder.hpp"

namespace soomla {
bool register_all_soomla_manual(se::Object* object) {
    register_cc_soomla_store_manual(object);
    register_cc_soomla_manual(object);
    register_cc_store_assets_builder_manual(object);
    register_cc_error_manual(object);
    register_cc_store_inventory_manual(object);
    register_cc_purchase_with_market_builder_manual(object);
    register_cc_lifetime_vg_builder_manual(object);
    register_cc_store_info_manual(object);
    register_cc_virtual_item_manual(object);
    register_cc_virtual_currency_pack_builder_manual(object);

    return true;
}
} // namespace soomla

#endif // __cplusplus
#endif // JSB_SOOMLA_HPP__
