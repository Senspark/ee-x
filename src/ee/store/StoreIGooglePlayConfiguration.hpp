//
//  StoreIGooglePlayConfiguration.hpp
//  Pods
//
//  Created by eps on 7/8/20.
//

#ifndef EE_X_STORE_I_GOOGLE_PLAY_CONFIGURATION_HPP
#define EE_X_STORE_I_GOOGLE_PLAY_CONFIGURATION_HPP

#ifdef __cplusplus

#include <string>

#include "ee/store/StoreIStoreConfiguration.hpp"

namespace ee {
namespace store {
class IGooglePlayConfiguration : public virtual IStoreConfiguration {
public:
    virtual void setPublicKey(const std::string& key) = 0;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_I_GOOGLE_PLAY_CONFIGURATION_HPP */
