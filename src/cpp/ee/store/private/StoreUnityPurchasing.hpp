//
//  StoreUnityPurchasing.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_UNITY_PURCHASING_HPP
#define EE_X_STORE_UNITY_PURCHASING_HPP

#include <memory>

#include "ee/store/StoreFwd.hpp"

namespace ee {
namespace store {
class UnityPurchasing {
public:
    static void
    initialize(const std::shared_ptr<IStoreListener>& listener,
               const ConfigurationBuilder& builder,
               const std::shared_ptr<ITransactionLog>& transactionLog);

    static void
    clearTransactionLog(const std::shared_ptr<ITransactionLog>& transactionLog);

private:
    static void
    initialize(const std::shared_ptr<IStoreListener>& listener,
               const ConfigurationBuilder& builder,
               const std::shared_ptr<ITransactionLog>& transactionLog,
               const Logger& logger);
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_UNITY_PURCHASING_HPP */
