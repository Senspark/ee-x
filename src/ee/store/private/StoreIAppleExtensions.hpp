//
//  StoreIAppleExtensions.hpp
//  Pods
//
//  Created by eps on 6/29/20.
//

#ifndef EE_X_STORE_I_APPLE_EXTENSIONS_HPP
#define EE_X_STORE_I_APPLE_EXTENSIONS_HPP

#include <functional>
#include <map>

#include "StoreIStoreExtension.hpp"

namespace ee {
namespace store {
class IAppleExtensions : IStoreExtension {
public:
    virtual void refreshAppReceipt(
        const std::function<void(const std::string&)>& successCallback,
        const std::function<void()>& errorCallback) = 0;

    virtual std::string getTransactionReceiptForProduct(
        const std::shared_ptr<Product>& product) = 0;

    virtual void
    restoreTransactions(const std::function<void(bool)>& callback) = 0;

    virtual void registerPurchaseDeferredListener(
        const std::function<void(const std::shared_ptr<Product>& product)>&
            callback) = 0;

    virtual bool simulateAskToBuy() const = 0;
    virtual void simulateAskToBuy(bool value) = 0;

    virtual std::map<std::string, std::string>
    getIntroductoryPriceDictionary() const = 0;
    virtual std::map<std::string, std::string> getProductDetails() const = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_APPLE_EXTENSIONS_HPP */
