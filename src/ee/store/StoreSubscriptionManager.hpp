//
//  StoreSubscriptionManager.hpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#ifndef EE_X_STORE_SUBSCRIPTION_MANAGER_HPP
#define EE_X_STORE_SUBSCRIPTION_MANAGER_HPP

#ifdef __cplusplus

#include <string>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class SubscriptionManager {
public:
    explicit SubscriptionManager(const std::shared_ptr<Product>& product,
                                 const std::string& introJson);

    explicit SubscriptionManager(const std::string& receipt,
                                 const std::string& id,
                                 const std::string& introJson);

    std::shared_ptr<SubscriptionInfo> getSubscriptionInfo();

private:
    std::shared_ptr<SubscriptionInfo>
    getAppleAppStoreSubInfo(const std::string& payload,
                            const std::string& productId);

    std::shared_ptr<SubscriptionInfo>
    getGooglePlayStoreSubInfo(const std::string& payload);

    std::string receipt_;
    std::string productId_;
    std::string introJson_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_SUBSCRIPTION_MANAGER_HPP */
