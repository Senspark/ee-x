//
//  StoreInfo.hpp
//  ee_x
//
//  Created by Zinge on 5/19/17.
//
//

#ifndef EE_X_STORE_STORE_INFO_HPP_
#define EE_X_STORE_STORE_INFO_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ee {
namespace store {
class VirtualItem;

class StoreInfo {
public:
    static StoreInfo& getInstance();

    VirtualItem* getItemByItemId(const std::string& itemId);

private:
    std::map<std::string, std::unique_ptr<VirtualItem>> virtualItems_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_STORE_INFO_HPP_ */
