//
//  Store.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#include "Store.hpp"

namespace ee {
namespace store {
Store& Store::getInstance() {
    static Store sharedInstance;
    return sharedInstance;
}

void Store::initialize(const StoreAssets& assets) {
    //
}
} // namespace store
} // namespace ee
