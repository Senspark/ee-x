//
//  StoreAppStore.hpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#ifndef EE_X_STORE_APP_STORE_HPP
#define EE_X_STORE_APP_STORE_HPP

#ifdef __cplusplus

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
enum class AppStore {
    NotSpecified,
    GooglePlay,
    AmazonAppStore,
    CloudMoolah,
    SamsungApps,
    XiaomiMiPay,
    UDP,
    MacAppStore,
    AppleAppStore,
    WinRT,
    TizenStore,
    FacebookStore,
    fake,
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_APP_STORE_HPP */
