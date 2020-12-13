//
//  StoreIController.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_I_STORE_CONTROLLER_HPP
#define EE_X_STORE_I_STORE_CONTROLLER_HPP

#include <string>

#include "ee/store/StoreFwd.hpp"

namespace ee {
namespace store {
class IStoreController {
public:
    virtual ~IStoreController() = default;

    virtual std::shared_ptr<ProductCollection> products() const = 0;
    virtual void initiatePurchase(const std::shared_ptr<Product>& product) = 0;
    virtual void initiatePurchase(const std::string& itemId) = 0;
    virtual void
    confirmPendingPurchase(const std::shared_ptr<Product>& product) = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_STORE_CONTROLLER_HPP */
