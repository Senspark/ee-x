//
//  StoreStandardPurchasingModule.hpp
//  ee-x-6914a733
//
//  Created by eps on 7/2/20.
//

#ifndef EE_X_STORE_STANDARD_PURCHASING_MODULE_HPP
#define EE_X_STORE_STANDARD_PURCHASING_MODULE_HPP

#ifdef __cplusplus

#include "ee/store/StoreAbstractPurchasingModule.hpp"
#include "ee/store/StoreIAndroidStoreSelection.hpp"

namespace ee {
namespace store {
class StandardPurchasingModule
    : public std::enable_shared_from_this<StandardPurchasingModule>,
      public AbstractPurchasingModule,
      public IAndroidStoreSelection {
private:
    using Self = StandardPurchasingModule;

    class StoreInstance;

public:
    virtual AppStore appStore() const override;

    bool useFakeStoreAlways() const;
    void useFakeStoreAlways(bool value);

    static std::shared_ptr<Self> instance();
    static std::shared_ptr<Self> instance(AppStore androidStore);

    virtual void configure() override;

private:
    explicit StandardPurchasingModule(
        const Logger& logger,
        const std::shared_ptr<INativeStoreProvider>& nativeStoreProvider,
        AppStore android);

    std::shared_ptr<StoreInstance> instantiateStore();
    std::shared_ptr<IStore> instantiateAndroid();
    std::shared_ptr<IStore> instantiateApple();
    std::shared_ptr<IStore> instantiateFakeStore();

    AppStore appStorePlatform_;
    std::shared_ptr<INativeStoreProvider> nativeStoreProvider_;
    static std::shared_ptr<Self> moduleInstance_;
    const Logger& logger_;
    std::shared_ptr<StoreInstance> storeInstance_;
    bool useFakeStoreAlways_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_STANDARD_PURCHASING_MODULE_HPP */
