//
//  SingleUsePackVG.hpp
//  ee_x
//
//  Created by Zinge on 5/19/17.
//
//

#ifndef EE_X_STORE_SINGLE_USE_PACK_VG_HPP_
#define EE_X_STORE_SINGLE_USE_PACK_VG_HPP_

#include "ee/store/VirtualGood.hpp"

namespace ee {
namespace store {
class SingleUsePackVG : public VirtualGood {
public:
    static std::unique_ptr<SingleUsePackVG>
    create(const std::string& itemId, const std::string& goodItemId,
           int goodAmount, std::unique_ptr<PurchaseType> purchaseType);

    const std::string& getGoodItemId() const;

    virtual int addBalance(int amount, bool notify) override;

private:
    std::string goodItemId_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_SINGLE_USE_PACK_VG_HPP_ */
