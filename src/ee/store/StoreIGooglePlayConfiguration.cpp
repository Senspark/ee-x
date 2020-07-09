//
//  StoreIGooglePlayConfiguration.cpp
//  Pods
//
//  Created by eps on 7/8/20.
//

#include "ee/store/StoreIGooglePlayConfiguration.hpp"

#include "ee/store/StoreTypeCache.hpp"

namespace ee {
namespace store {
using Self = IGooglePlayConfiguration;

template <>
std::string TypeCache::toString<Self>() {
    return "IGooglePlayConfiguration";
}
} // namespace store
} // namespace ee
