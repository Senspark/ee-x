//
//  StoreTypeCache.cpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#include "ee/store/private/StoreTypeCache.hpp"

#include "ee/store/private/StoreIAppleExtensions.hpp"
#include "ee/store/private/StoreIGooglePlayConfiguration.hpp"
#include "ee/store/private/StoreIGooglePlayStoreExtensions.hpp"
#include "ee/store/private/StoreITransactionHistoryExtensions.hpp"

namespace ee {
namespace store {
using Self = TypeCache;

template <>
std::string Self::toString<IAppleExtensions>() {
    return "StoreIAppleExtensions";
}

template <>
std::string Self::toString<IGooglePlayConfiguration>() {
    return "IGooglePlayConfiguration";
}

template <>
std::string Self::toString<IGooglePlayStoreExtensions>() {
    return "IGooglePlayStoreExtensions";
}

template <>
std::string Self::toString<ITransactionHistoryExtensions>() {
    return "ITransactionHistoryExtensions";
}
} // namespace store
} // namespace ee
