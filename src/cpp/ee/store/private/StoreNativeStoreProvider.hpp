//
//  StoreNativeStoreProvider.hpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#ifndef EE_X_STORE_NATIVE_STORE_PROVIDER_HPP
#define EE_X_STORE_NATIVE_STORE_PROVIDER_HPP

#include "ee/store/private/StoreINativeStoreProvider.hpp"

namespace ee {
namespace store {
class NativeStoreProvider : public INativeStoreProvider {
public:
    explicit NativeStoreProvider(IMessageBridge& bridge);

    virtual std::shared_ptr<INativeStore> getAndroidStore(
        const std::shared_ptr<IUnityCallback>& callback, AppStore store,
        const std::shared_ptr<IPurchasingBinder>& binder) const override;

    virtual std::shared_ptr<INativeAppleStore>
    getStoreKit(const std::shared_ptr<IUnityCallback>& callback) const override;

private:
    IMessageBridge& bridge_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_NATIVE_STORE_PROVIDER_HPP */
