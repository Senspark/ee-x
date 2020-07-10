//
//  StoreIPurchaseReceipt.hpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#ifndef EE_X_STORE_I_PURCHASE_RECEIPT_HPP
#define EE_X_STORE_I_PURCHASE_RECEIPT_HPP

#include <string>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class IPurchaseReceipt {
public:
    virtual ~IPurchaseReceipt() = default;

    virtual std::string transactionId() const = 0;
    virtual std::string productId() const = 0;
    virtual int purchaseDate() const = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_PURCHASE_RECEIPT_HPP */
