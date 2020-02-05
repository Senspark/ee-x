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

#ifndef __CCNativeVirtualCurrencyStorage_H_
#define __CCNativeVirtualCurrencyStorage_H_

#include "Soomla/data/CCVirtualCurrencyStorage.h"

namespace soomla {
/**
 Implements the `CCVirtualCurrencyStorage` using the bridge to talk
 with the native implementation of VirtualCurrencyStorage

 See parent for all functions.
 */
class CCNativeVirtualCurrencyStorage : public CCVirtualCurrencyStorage {
public:
    CCNativeVirtualCurrencyStorage();

    virtual ~CCNativeVirtualCurrencyStorage();

    virtual int getBalance(CCVirtualItem* item, CCError** error = nullptr);

    virtual int setBalance(CCVirtualItem* item, int balance, bool notify,
                           CCError** error = nullptr);

    virtual int add(CCVirtualItem* item, int amount, bool notify,
                    CCError** error = nullptr);

    virtual int remove(CCVirtualItem* item, int amount, bool notify,
                       CCError** error = nullptr);
};
} // namespace soomla

#endif // __CCNativeVirtualCurrencyStorage_H_
