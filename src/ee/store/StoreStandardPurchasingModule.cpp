//
//  StoreStandardPurchasingModule.cpp
//  ee-x-6914a733
//
//  Created by eps on 7/2/20.
//

#include "ee/store/StoreStandardPurchasingModule.hpp"

#include <ee/core/Logger.hpp>
#include <ee/core/internal/MessageBridge.hpp>

#include "ee/store/StoreAppStore.hpp"
#include "ee/store/private/StoreAppleStoreImpl.hpp"
#include "ee/store/private/StoreNativeStoreProvider.hpp"

namespace ee {
namespace store {
using Self = StandardPurchasingModule;

class Self::StoreInstance {
public:
    explicit StoreInstance(const std::string& name,
                           const std::shared_ptr<IStore>& instance);

    const std::string& storeName() const;
    const std::shared_ptr<IStore>& instance() const;

private:
    std::string storeName_;
    std::shared_ptr<IStore> instance_;
};

Self::StoreInstance::StoreInstance(const std::string& name,
                                   const std::shared_ptr<IStore>& instance)
    : storeName_(name)
    , instance_(instance) {}

const std::string& Self::StoreInstance::storeName() const {
    return storeName_;
}

const std::shared_ptr<IStore>& Self::StoreInstance::instance() const {
    return instance_;
}

std::shared_ptr<Self> Self::moduleInstance_;

Self::StandardPurchasingModule(
    const Logger& logger,
    const std::shared_ptr<INativeStoreProvider>& nativeStoreProvider,
    AppStore android)
    : logger_(logger) {
    nativeStoreProvider_ = nativeStoreProvider;
    useFakeStoreAlways_ = false;
    appStorePlatform_ = android;
}

AppStore Self::appStore() const {
    return appStorePlatform_;
}

bool Self::useFakeStoreAlways() const {
    return useFakeStoreAlways_;
}

void Self::useFakeStoreAlways(bool value) {
    useFakeStoreAlways_ = value;
}

std::shared_ptr<Self> Self::instance() {
    return instance(AppStore::NotSpecified);
}

std::shared_ptr<Self> Self::instance(AppStore androidStore) {
    if (moduleInstance_ == nullptr) {
        if (androidStore == AppStore::NotSpecified) {
#ifdef EE_X_ANDROID
            androidStore = AppStore::GooglePlay;
#endif // EE_X_ANDROID
#ifdef EE_X_IOS
            androidStore = AppStore::AppleAppStore;
#endif // EE_X_IOS
        }
        auto&& bridge = MessageBridge::getInstance();
        moduleInstance_ = std::shared_ptr<Self>(new Self(
            Logger::getSystemLogger(),
            std::make_shared<NativeStoreProvider>(bridge), androidStore));
    }
    return moduleInstance_;
}

void Self::configure() {
    if (storeInstance_ == nullptr) {
        storeInstance_ = instantiateStore();
    }
    registerStore(storeInstance_->storeName(), storeInstance_->instance());
    if (auto instance = std::dynamic_pointer_cast<IStoreInternal>(
            storeInstance_->instance());
        instance != nullptr) {
        instance->setModule(shared_from_this());
    }
    if (auto instance =
            std::dynamic_pointer_cast<ITransactionHistoryExtensions>(
                storeInstance_->instance());
        instance != nullptr) {
        bindExtension<ITransactionHistoryExtensions>(instance);
    }
}

std::shared_ptr<Self::StoreInstance> Self::instantiateStore() {
    if (useFakeStoreAlways_) {
        return std::make_shared<StoreInstance>("fake", instantiateFakeStore());
    }
#ifdef EE_X_IOS
    return std::make_shared<StoreInstance>("AppleAppStore", instantiateApple());
#endif // EE_X_IOS
#ifdef EE_X_ANDROID
    return std::make_shared<StoreInstance>("GooglePlay", instantiateAndroid());
#endif // EE_X_ANDROID
}

std::shared_ptr<IStore> Self::instantiateAndroid() {
    auto store = std::make_shared<JsonStore>();
    auto androidStore = nativeStoreProvider_->getAndroidStore(
        store, appStorePlatform_, binder_);
    store->setNativeStore(androidStore);
    return store;
}

std::shared_ptr<IStore> Self::instantiateApple() {
    auto appleStoreImpl = std::make_shared<AppleStoreImpl>();
    auto storeKit = nativeStoreProvider_->getStoreKit(appleStoreImpl);
    appleStoreImpl->setNativeStore(storeKit);
    bindExtension<IAppleExtensions>(appleStoreImpl);
    return appleStoreImpl;
}

std::shared_ptr<IStore> Self::instantiateFakeStore() {
    // FIXME.
    return nullptr;
}
} // namespace store
} // namespace ee
