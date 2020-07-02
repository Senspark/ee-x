//
//  StoreIAppleExtensions.cpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#include "ee/store/StoreIAppleExtensions.hpp"

#include "ee/store/StoreTypeCache.hpp"

namespace ee {
namespace store {
using Self = IAppleExtensions;

template <>
std::string TypeCache::toString<Self>() {
    return "IAppleExtensions";
}
} // namespace store
} // namespace ee
