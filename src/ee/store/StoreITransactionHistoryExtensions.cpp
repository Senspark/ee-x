//
//  StoreITransactionHistoryExtensions.cpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#include "ee/store/StoreITransactionHistoryExtensions.hpp"

#include "ee/store/StoreTypeCache.hpp"

namespace ee {
namespace store {
using Self = ITransactionHistoryExtensions;

template <>
std::string TypeCache::toString<Self>() {
    return "ITransactionHistoryExtensions";
}
} // namespace store
} // namespace ee
