//
//  StoreJsonSerializer.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_JSON_SERIALIZER_HPP
#define EE_X_STORE_JSON_SERIALIZER_HPP

#include <map>
#include <string>
#include <vector>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class JsonSerializer {
public:
    static std::string serializeProductDefinition(
        const std::shared_ptr<ProductDefinition>& product);

    static std::string serializeProductDefinitions(
        const std::vector<std::shared_ptr<ProductDefinition>>& products);

    static std::string serializeProductDescription(
        const std::shared_ptr<ProductDescription>& product);

    static std::string serializeProductDescriptions(
        const std::vector<std::shared_ptr<ProductDescription>>& products);

    static std::vector<std::shared_ptr<ProductDescription>>
    deserializeProductDescriptions(const std::string& json);

    static std::map<std::string, std::string>
    deserializeSubscriptionDescriptions(const std::string& json);

    static std::map<std::string, std::string>
    deserializeProductDetails(const std::string& json);

    static PurchaseFailureDescription
    deserializeFailureReason(const std::string& json);
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_JSON_SERIALIZER_HPP */
