/*
 Copyright (C) 2012-2014 Soomla Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef __CCVirtualCurrencyStorage_H__
#define __CCVirtualCurrencyStorage_H__

#include "soomla/data/CCVirtualItemStorage.h"

namespace soomla {
/**
 This class is a definition of a Virtual Currency Storage.
 */
class CCVirtualCurrencyStorage : public CCVirtualItemStorage {
public:
    static CCVirtualCurrencyStorage* getInstance();

    CCVirtualCurrencyStorage();

    virtual ~CCVirtualCurrencyStorage();

protected:
    virtual const char* keyBalance(const char* itemId) const;
    virtual void postBalanceChangeEvent(CCVirtualItem* item, int balance,
                                        int amountAdded);

private:
    static const char* keyCurrencyBalance(const char* itemId);
};
} // namespace soomla
#endif // __CCVirtualCurrencyStorage_H__
