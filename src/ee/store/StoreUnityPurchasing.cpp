//
//  StoreUnityPurchasing.cpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#include "ee/store/StoreUnityPurchasing.hpp"

#include <ee/core/Logger.hpp>

#include "ee/store/StoreConfigurationBuilder.hpp"
#include "ee/store/StoreITransactionLog.hpp"
#include "ee/store/private/StorePurchasingFactory.hpp"
#include "ee/store/private/StorePurchasingManager.hpp"
#include "ee/store/private/StoreStoreListenerProxy.hpp"

namespace ee {
namespace store {
using Self = UnityPurchasing;

void Self::initialize(const std::shared_ptr<IStoreListener>& listener,
                      const ConfigurationBuilder& builder,
                      const std::shared_ptr<ITransactionLog>& transactionLog) {
    initialize(listener, builder, transactionLog, Logger::getSystemLogger());
}

void Self::clearTransactionLog(
    const std::shared_ptr<ITransactionLog>& transactionLog) {
    transactionLog->clear();
}

void Self::initialize(const std::shared_ptr<IStoreListener>& listener,
                      const ConfigurationBuilder& builder,
                      const std::shared_ptr<ITransactionLog>& transactionLog,
                      const Logger& logger) {
    auto manager = std::make_shared<PurchasingManager>(
        transactionLog, logger, builder.factory_->service(),
        builder.factory_->storeName());
    auto proxy =
        std::make_shared<StoreListenerProxy>(listener, builder.factory_);
    manager->initialize(proxy, builder.products());
}
} // namespace store
} // namespace ee
