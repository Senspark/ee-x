//
//  StoreScriptingUnityCallback.hpp
//  Pods
//
//  Created by eps on 7/8/20.
//

#ifndef EE_X_STORE_SCRIPTING_UNITY_CALLBACK_HPP
#define EE_X_STORE_SCRIPTING_UNITY_CALLBACK_HPP

#include "ee/store/private/StoreIUnityCallback.hpp"

namespace ee {
namespace store {
class ScriptingUnityCallback : public IUnityCallback {
public:
    explicit ScriptingUnityCallback(
        const std::shared_ptr<IUnityCallback>& forwardTo);

    virtual void onSetupFailed(const std::string& json) override;
    virtual void onProductsRetrieved(const std::string& json) override;
    virtual void onPurchaseSucceeded(const std::string& id,
                                     const std::string& receipt,
                                     const std::string& transactionId) override;
    virtual void onPurchaseFailed(const std::string& json) override;

private:
    std::shared_ptr<IUnityCallback> forwardTo_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_SCRIPTING_UNITY_CALLBACK_HPP */
