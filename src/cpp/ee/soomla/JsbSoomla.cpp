//
//  jsb_soomla.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/soomla/JsbSoomla.hpp"

#include "ee/soomla/internal/JsbVirtualCurrency.hpp"
#include "ee/soomla/internal/jsb_cc_error.hpp"
#include "ee/soomla/internal/jsb_cc_lifetime_vg.hpp"
#include "ee/soomla/internal/jsb_cc_lifetime_vg_builder.hpp"
#include "ee/soomla/internal/jsb_cc_market_item.hpp"
#include "ee/soomla/internal/jsb_cc_purchasable_virtual_item.hpp"
#include "ee/soomla/internal/jsb_cc_purchase_with_market.hpp"
#include "ee/soomla/internal/jsb_cc_purchase_with_market_builder.hpp"
#include "ee/soomla/internal/jsb_cc_single_use_pack_vg.hpp"
#include "ee/soomla/internal/jsb_cc_soomla.hpp"
#include "ee/soomla/internal/jsb_cc_soomla_store.hpp"
#include "ee/soomla/internal/jsb_cc_store_assets_builder.hpp"
#include "ee/soomla/internal/jsb_cc_store_info.hpp"
#include "ee/soomla/internal/jsb_cc_store_inventory.hpp"
#include "ee/soomla/internal/jsb_cc_virtual_currency_builder.hpp"
#include "ee/soomla/internal/jsb_cc_virtual_currency_pack.hpp"
#include "ee/soomla/internal/jsb_cc_virtual_currency_pack_builder.hpp"
#include "ee/soomla/internal/jsb_cc_virtual_item.hpp"
#include "ee/soomla/internal/jsb_store_event_listener.hpp"

namespace soomla {
bool registerJsb(se::Object* global) {
    register_cc_soomla_store_manual(global);
    register_cc_soomla_manual(global);
    register_cc_store_assets_builder_manual(global);
    register_cc_error_manual(global);
    register_cc_store_inventory_manual(global);
    register_cc_market_item_manual(global);
    register_cc_purchase_with_market_manual(global);
    register_cc_purchase_with_market_builder_manual(global);
    register_cc_virtual_item_manual(global);
    registerVirtualCurrency(global);
    register_cc_store_info_manual(global);
    register_cc_purchasable_virtual_item_manual(global);
    register_cc_single_use_pack_vg_manual(global);
    register_cc_lifetime_vg_manual(global);
    register_cc_lifetime_vg_builder_manual(global);
    register_cc_virtual_currency_pack_manual(global);
    register_cc_virtual_currency_pack_builder_manual(global);
    register_store_event_listener_manual(global);
    register_cc_virtual_currency_builder_manual(global);
    return true;
}
} // namespace soomla
