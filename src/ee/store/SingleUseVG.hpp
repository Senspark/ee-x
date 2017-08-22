//
//  SingleUseVG.hpp
//  ee_x
//
//  Created by Zinge on 5/19/17.
//
//

#ifndef EE_X_STORE_SINGLE_USE_VG_HPP_
#define EE_X_STORE_SINGLE_USE_VG_HPP_

#include "ee/store/VirtualGood.hpp"

namespace ee {
namespace store {
class PurchaseType;

class SingleUseVG : public VirtualGood {
public:
    static std::unique_ptr<SingleUseVG>
    create(const std::string& itemId,
           const std::unique_ptr<PurchaseType> purchaseType);

    virtual bool canBuy() override;

    virtual int addBalance(int amount, bool notify) override;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_SINGLE_USE_VG_HPP_ */
