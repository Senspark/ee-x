//
//  VirtualItem.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_VIRTUAL_ITEM_HPP_
#define EE_X_STORE_VIRTUAL_ITEM_HPP_

#include <string>

namespace ee {
namespace store {
class VirtualItem {
public:
    VirtualItem();

    virtual ~VirtualItem();

    const std::string& getItemId() const;

    int give(int amount);

    virtual int give(int amount, bool notify) = 0;

    int take(int amount);

    virtual int take(int amount, bool notify) = 0;

    int resetBalance(int balance);

    virtual int resetBalance(int balance, bool notify) = 0;

    virtual int getBalance() = 0;

    virtual void save(bool saveToDb = true);

private:
};
}
}

#endif /* EE_X_STORE_VIRTUAL_ITEM_HPP_ */
