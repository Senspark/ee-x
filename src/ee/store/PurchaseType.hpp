//
//  PurchaseType.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_PURCHASE_TYPE_HPP_
#define EE_X_STORE_PURCHASE_TYPE_HPP_

#include <string>

namespace ee {
namespace store {
class Error;

class PurchaseType {
public:
    PurchaseType();

    virtual ~PurchaseType();

    virtual bool init();

    virtual void buy(const std::string& payload, Error** error = nullptr) = 0;

    virtual bool canAfford(Error** error = nullptr) = 0;

    const std::string& getAssociatedItemId() const;

    void setAssociatedItemId(const std::string& itemId);

private:
    std::string associatedItemId_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_PURCHASE_TYPE_HPP_ */
