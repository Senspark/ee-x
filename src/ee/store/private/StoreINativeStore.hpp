//
//  StoreINativeStore.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_I_NATIVE_STORE_HPP
#define EE_X_STORE_I_NATIVE_STORE_HPP

#include <string>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class INativeStore {
public:
    virtual ~INativeStore() = default;
    virtual void retrieveProducts(const std::string& json) = 0;
    virtual void purchase(const std::string& productJson) = 0;
    virtual void finishTransaction(const std::string& productJson,
                                   const std::string& transactionId) = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_NATIVE_STORE_HPP */
