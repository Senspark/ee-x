//
//  VirtualCurrencyPack.hpp
//  ee_x
//
//  Created by Zinge on 5/18/17.
//
//

#ifndef EE_X_STORE_VIRTUAL_CURRENCY_PACK_HPP_
#define EE_X_STORE_VIRTUAL_CURRENCY_PACK_HPP_

#include "ee/store/PurchasableVirtualItem.hpp"

namespace ee {
namespace store {
class PurchaseType;

class VirtualCurrencyPack : public PurchasableVirtualItem {
public:
    static std::unique_ptr<VirtualCurrencyPack>
    create(const std::string& itemId, const std::string& currencyItemId,
           int currencyAmount, std::unique_ptr<PurchaseType> purchaseType);
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_VIRTUAL_CURRENCY_PACK_HPP_ */
