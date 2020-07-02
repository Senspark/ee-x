//
//  StoreIStoreConfiguration.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_I_STORE_CONFIGURATION_HPP
#define EE_X_STORE_I_STORE_CONFIGURATION_HPP

#ifdef __cplusplus

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class IStoreConfiguration {
public:
    virtual ~IStoreConfiguration() = default;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_I_STORE_CONFIGURATION_HPP */
