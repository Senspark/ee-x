//
//  Store.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_BRIDGE_HPP
#define EE_X_STORE_BRIDGE_HPP

#include <optional>
#include <string>
#include <vector>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
enum SkuType {
    InApp,
    Subscription,
};

class Bridge {
public:
    Bridge();
    ~Bridge();

    explicit Bridge(const Logger& logger);

    void destroy();

    void initialize();

    Task<std::vector<SkuDetails>>
    getSkuDetails(SkuType type, const std::vector<std::string>& skuList);

    Task<std::vector<Purchase>> getPurchases(SkuType type);

    Task<std::vector<PurchaseHistoryRecord>> getPurchaseHistory(SkuType type);

    Task<std::optional<Purchase>> purchase(const std::string& sku);

    Task<bool> consume(const std::string& purchaseToken);

    Task<bool> acknowledge(const std::string& purchaseToken);

private:
    IMessageBridge& bridge_;
    const Logger& logger_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_BRIDGE_HPP */
