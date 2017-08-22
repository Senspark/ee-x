//
//  StoreInfo.cpp
//  ee_x
//
//  Created by Zinge on 5/19/17.
//
//

#include "ee/store/StoreInfo.hpp"
#include "ee/store/StoreLogger.hpp"
#include "ee/store/StoreUtils.hpp"

namespace ee {
namespace store {
namespace {
constexpr auto Tag = "EEX StoreInfo";
} // namespace

VirtualItem* StoreInfo::getItemByItemId(const std::string& itemId) {
    auto iter = virtualItems_.find(itemId);
    if (iter == virtualItems_.cend()) {
        StoreLogger::getInstance().logError(
            Tag,
            format("Virtual item with ID = %s was not found", itemId.c_str()));
        return nullptr;
    }
    return iter->second.get();
}
} // namespace store
} // namespace ee
