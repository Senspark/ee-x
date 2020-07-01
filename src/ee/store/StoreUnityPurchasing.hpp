//
//  StoreUnityPurchasing.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_UNITY_PURCHASING_HPP
#define EE_X_STORE_UNITY_PURCHASING_HPP

#ifdef __cplusplus

#include <memory>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class UnityPurchasing {
public:
    static void
    initialize(const std::shared_ptr<IStoreListener>& listener,
               const std::shared_ptr<ConfigurationBuilder>& builder,
               const std::shared_ptr<ITransactionLog>& transactionLog);

    static void
    clearTransactionLog(const std::shared_ptr<ITransactionLog>& transactionLog);

private:
    static void
    initialize(const std::shared_ptr<IStoreListener>& listener,
               const std::shared_ptr<ConfigurationBuilder>& builder,
               const std::shared_ptr<ITransactionLog>& transactionLog,
               const Logger& logger);
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_UNITY_PURCHASING_HPP */
