//
//  VirtualGood.hpp
//  ee_x
//
//  Created by Zinge on 5/18/17.
//
//

#ifndef EE_X_STORE_VIRTUAL_GOOD_HPP_
#define EE_X_STORE_VIRTUAL_GOOD_HPP_

#include "ee/store/PurchasableVirtualItem.hpp"

namespace ee {
namespace store {
class VirtualGood : public PurchasableVirtualItem {
public:
    virtual int getBalance() override;

    virtual int setBalance(int balance, bool notify) override;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_VIRTUAL_GOOD_HPP_ */
