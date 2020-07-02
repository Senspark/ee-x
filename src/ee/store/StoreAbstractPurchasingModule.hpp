//
//  AbstractPurchasingModule.hpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#ifndef EE_X_STORE_ABSTRACT_PURCHASING_MODULE_HPP
#define EE_X_STORE_ABSTRACT_PURCHASING_MODULE_HPP

#ifdef __cplusplus

#include "ee/store/StoreIPurchasingBinder.hpp"
#include "ee/store/StoreIPurchasingModule.hpp"

namespace ee {
namespace store {
class AbstractPurchasingModule : public IPurchasingModule {
public:
    virtual void
    configure(const std::shared_ptr<IPurchasingBinder>& binder) override;

    virtual void configure() = 0;

protected:
    void registerStore(const std::string& name,
                       const std::shared_ptr<IStore>& store);

    template <class T>
    void bindExtension(const std::shared_ptr<IStoreExtension>& instance) {
        binder_->registerExtension<T>(instance);
    }

    template <class T>
    void
    bindConfiguration(const std::shared_ptr<IStoreConfiguration>& instance) {
        binder_->registerConfiguration<T>(instance);
    }

    std::shared_ptr<IPurchasingBinder> binder_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_ABSTRACT_PURCHASING_MODULE_HPP */
