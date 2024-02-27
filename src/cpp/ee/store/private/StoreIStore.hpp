//
//  StoreIStore.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_I_STORE_HPP
#define EE_X_STORE_I_STORE_HPP

#include <vector>

#include "ee/store/StoreFwd.hpp"

namespace ee {
namespace store {
class IStore {
public:
    virtual ~IStore() = default;

    virtual void initialize(const std::shared_ptr<IStoreCallback> &callback) = 0;

    virtual void retrieveProducts(const std::vector<std::shared_ptr<ProductDefinition>> &products) = 0;

    virtual void purchase(const std::shared_ptr<ProductDefinition> &product) = 0;

    virtual void finishTransaction(const std::shared_ptr<ProductDefinition> &product,
                                   const std::string &transactionId) = 0;

    virtual void setObfuscationAccountId(const std::string& obfuscationAccountId) = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_STORE_HPP */
