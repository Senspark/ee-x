//
//  StoreNativeStoreProvider.cpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#include "ee/store/private/StoreNativeStoreProvider.hpp"

#include <cassert>

#include "ee/store/StoreAppStore.hpp"
#include "ee/store/private/StoreAndroidJavaStore.hpp"
#include "ee/store/private/StoreIPurchasingBinder.hpp"
#include "ee/store/private/StoreIosStoreBindings.hpp"

namespace ee {
namespace store {
using Self = NativeStoreProvider;

Self::NativeStoreProvider(IMessageBridge& bridge)
    : bridge_(bridge) {}

std::shared_ptr<INativeStore>
Self::getAndroidStore(const std::shared_ptr<IUnityCallback>& callback,
                      AppStore store,
                      const std::shared_ptr<IPurchasingBinder>& binder) const {
    if (store == AppStore::GooglePlay) {
        auto storeBridge = std::make_shared<AndroidJavaStore>(bridge_, callback);
        binder->registerExtension<IGooglePlayStoreExtensions>(storeBridge);
        binder->registerConfiguration<IGooglePlayConfiguration>(storeBridge);
        return storeBridge;
    }
    // FIXME.
    assert(false);
    return nullptr;
}

std::shared_ptr<INativeAppleStore>
Self::getStoreKit(const std::shared_ptr<IUnityCallback>& callback) const {
#ifdef EE_X_IOS
    return std::make_shared<IosStoreBindings>(bridge_);
#endif // EE_X_IOS
    // FIXME.
    assert(false);
    return nullptr;
}
} // namespace store
} // namespace ee
