//
//  VirtualCurrencyStorage.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_VIRTUAL_CURRENCY_STORAGE_HPP_
#define EE_X_STORE_VIRTUAL_CURRENCY_STORAGE_HPP_

#include "ee/store/VirtualItemStorage.hpp"

namespace ee {
namespace store {
class VirtualCurrencyStorage : public VirtualItemStorage {
public:
    static VirtualCurrencyStorage& getInstance();

protected:
    VirtualCurrencyStorage() = default;
    virtual ~VirtualCurrencyStorage() = default;

    virtual std::string keyBalance(const std::string& itemId) const override;

    virtual void postBalanceChangeEvent(VirtualItem* item, int balance,
                                        int amount) override;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_VIRTUAL_CURRENCY_STORAGE_HPP_ */
