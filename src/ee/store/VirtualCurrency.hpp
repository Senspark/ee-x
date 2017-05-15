//
//  VirtualCurrency.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_VIRTUAL_CURRENCY_HPP_
#define EE_X_STORE_VIRTUAL_CURRENCY_HPP_

#include <memory>

#include "ee/store/VirtualItem.hpp"

namespace ee {
namespace store {
class VirtualCurrency : public VirtualItem {
public:
    static std::unique_ptr<VirtualCurrency> create(const std::string& itemId);

    virtual int give(int amount, bool notify) override;

    virtual int take(int amount, bool notify) override;

    virtual int resetBalance(int balance, bool notify) override;

    virtual int getBalance() override;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_VIRTUAL_CURRENCY_HPP_ */
