//
//  StoreIGooglePlayStoreExtensions.hpp
//  Pods
//
//  Created by eps on 7/8/20.
//

#ifndef EE_X_STORE_I_GOOGLE_PLAY_STORE_EXTENSIONS_HPP
#define EE_X_STORE_I_GOOGLE_PLAY_STORE_EXTENSIONS_HPP

#include <map>
#include <string>

#include "ee/store/private/StoreIStoreExtension.hpp"

namespace ee {
namespace store {
class IGooglePlayStoreExtensions : public virtual IStoreExtension {
public:
    virtual std::map<std::string, std::string>
    getProductJsonDictionary() const = 0;
    virtual void
    restoreTransactions(const std::function<void(bool)>& callback) = 0;
    virtual void
    finishAdditionalTransaction(const std::string& productId,
                                const std::string& transactionId) = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_GOOGLE_PLAY_STORE_EXTENSIONS_HPP */
