//
//  Store.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_BRIDGE_HPP
#define EE_X_STORE_BRIDGE_HPP

#ifdef __cplusplus

#include <optional>
#include <string>
#include <vector>

#include <ee/core/IPlugin.hpp>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
enum class SkuType {
    InApp,
    Subscription,
};

class Bridge final : public IPlugin {
public:
    Bridge();
    ~Bridge();

    explicit Bridge(const Logger& logger);

    virtual void destroy() override;

    void initialize();

    Task<std::optional<std::vector<SkuDetails>>>
    getSkuDetails(SkuType type, const std::vector<std::string>& skuList);

    Task<std::optional<std::vector<Purchase>>> getPurchases(SkuType type);

    Task<std::optional<std::vector<PurchaseHistoryRecord>>>
    getPurchaseHistory(SkuType type);

    Task<std::optional<Purchase>> purchase(const std::string& sku);

    Task<bool> consume(const std::string& purchaseToken);

    Task<bool> acknowledge(const std::string& purchaseToken);

private:
    IMessageBridge& bridge_;
    const Logger& logger_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_BRIDGE_HPP */
