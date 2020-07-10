//
//  StoreTypeCache.hpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#ifndef EE_X_STORE_TYPE_CACHE_HPP
#define EE_X_STORE_TYPE_CACHE_HPP

#include <string>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class TypeCache {
public:
    template <class T>
    static std::string toString();
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_TYPE_CACHE_HPP */
