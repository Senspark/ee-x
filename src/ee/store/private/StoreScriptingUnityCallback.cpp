//
//  StoreScriptingUnityCallback.cpp
//  Pods
//
//  Created by eps on 7/8/20.
//

#include "ee/store/private/StoreScriptingUnityCallback.hpp"

#include <ee/core/Thread.hpp>

namespace ee {
namespace store {
using Self = ScriptingUnityCallback;

Self::ScriptingUnityCallback(const std::shared_ptr<IUnityCallback>& forwardTo)
    : forwardTo_(forwardTo) {}

void Self::onSetupFailed(const std::string& json) {
    Thread::runOnLibraryThread([this, json] { //
        forwardTo_->onSetupFailed(json);
    });
}

void Self::onProductsRetrieved(const std::string& json) {
    Thread::runOnLibraryThread([this, json] { //
        forwardTo_->onProductsRetrieved(json);
    });
}

void Self::onPurchaseSucceeded(const std::string& id,
                               const std::string& receipt,
                               const std::string& transactionId) {
    Thread::runOnLibraryThread([this, id, receipt, transactionId] {
        forwardTo_->onPurchaseSucceeded(id, receipt, transactionId);
    });
}

void Self::onPurchaseFailed(const std::string& json) {
    Thread::runOnLibraryThread([this, json] { //
        forwardTo_->onPurchaseFailed(json);
    });
}
} // namespace store
} // namespace ee
