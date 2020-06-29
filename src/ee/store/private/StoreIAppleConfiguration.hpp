//
//  StoreIAppleConfiguration.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_I_APPLE_CONFIGURATION_HPP
#define EE_X_STORE_I_APPLE_CONFIGURATION_HPP

#include <string>

#include "StoreIStoreConfiguration.hpp"

namespace ee {
namespace store {
class IAppleConfiguration : public IStoreConfiguration {
public:
    virtual std::string appReceipt() const = 0;
    virtual bool canMakePayments() const = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_APPLE_CONFIGURATION_HPP */
