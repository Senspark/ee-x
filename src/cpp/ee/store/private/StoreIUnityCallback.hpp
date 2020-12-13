//
//  StoreIUnityCallback.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_I_UNITY_CALLBACK_HPP
#define EE_X_STORE_I_UNITY_CALLBACK_HPP

#include <string>

#include "ee/store/StoreFwd.hpp"

namespace ee {
namespace store {
class IUnityCallback {
public:
    virtual ~IUnityCallback() = default;

    virtual void onSetupFailed(const std::string& json) = 0;
    virtual void onProductsRetrieved(const std::string& json) = 0;
    virtual void onPurchaseSucceeded(const std::string& id,
                                     const std::string& receipt,
                                     const std::string& transactionId) = 0;
    virtual void onPurchaseFailed(const std::string& json) = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_UNITY_CALLBACK_HPP */
