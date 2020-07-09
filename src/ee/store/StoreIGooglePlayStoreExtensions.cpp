//
//  StoreIGooglePlayExtensions.cpp
//  Pods
//
//  Created by eps on 7/8/20.
//

#include "ee/store/StoreIGooglePlayStoreExtensions.hpp"

#include "ee/store/StoreTypeCache.hpp"

namespace ee {
namespace store {
using Self = IGooglePlayStoreExtensions;

template <>
std::string TypeCache::toString<Self>() {
    return "IGooglePlayStoreExtensions";
}
} // namespace store
} // namespace ee
