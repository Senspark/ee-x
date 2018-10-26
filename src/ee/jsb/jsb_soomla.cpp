//
//  jsb_soomla.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/jsb_soomla.hpp"

#include "ee/jsb/soomla/jsb_cc_error.hpp"
#include "ee/jsb/soomla/jsb_cc_life_time_vg.hpp"
#include "ee/jsb/soomla/jsb_cc_lifetime_vg_builder.hpp"
#include "ee/jsb/soomla/jsb_cc_market_item.hpp"
#include "ee/jsb/soomla/jsb_cc_purchasable_virtual_item.hpp"
#include "ee/jsb/soomla/jsb_cc_purchase_with_market.hpp"
#include "ee/jsb/soomla/jsb_cc_purchase_with_market_builder.hpp"
#include "ee/jsb/soomla/jsb_cc_single_use_pack_vg.hpp"
#include "ee/jsb/soomla/jsb_cc_soomla.hpp"
#include "ee/jsb/soomla/jsb_cc_soomla_store.hpp"
#include "ee/jsb/soomla/jsb_cc_store_assets_builder.hpp"
#include "ee/jsb/soomla/jsb_cc_store_info.hpp"
#include "ee/jsb/soomla/jsb_cc_store_inventory.hpp"
#include "ee/jsb/soomla/jsb_cc_virtual_currency_builder.hpp"
#include "ee/jsb/soomla/jsb_cc_virtual_currency_pack.hpp"
#include "ee/jsb/soomla/jsb_cc_virtual_currency_pack_builder.hpp"
#include "ee/jsb/soomla/jsb_cc_virtual_item.hpp"
#include "ee/jsb/soomla/jsb_store_event_listener.hpp"

namespace soomla {
bool register_all_soomla_manual(se::Object* object) {
    register_cc_soomla_store_manual(object);
    register_cc_soomla_manual(object);
    register_cc_store_assets_builder_manual(object);
    register_cc_error_manual(object);
    register_cc_store_inventory_manual(object);
    register_cc_market_item_manual(object);
    register_cc_purchase_with_market_manual(object);
    register_cc_purchase_with_market_builder_manual(object);
    register_cc_virtual_item_manual(object);
    register_cc_store_info_manual(object);
    register_cc_purchasable_virtual_item_manual(object);
    register_cc_single_use_pack_vg_manual(object);
    register_cc_life_time_vg_manual(object);
    register_cc_lifetime_vg_builder_manual(object);
    register_cc_virtual_currency_pack_manual(object);
    register_cc_virtual_currency_pack_builder_manual(object);
    register_store_event_listener_manual(object);
    register_cc_virtual_currency_builder_manual(object);

    return true;
}
} // namespace soomla
