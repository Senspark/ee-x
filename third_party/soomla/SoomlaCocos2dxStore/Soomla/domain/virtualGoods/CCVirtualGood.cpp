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

#include "CCVirtualGood.h"
#include "CCVirtualGoodsStorage.h"

namespace soomla {
    
    USING_NS_CC;
    
    int CCVirtualGood::resetBalance(int balance, bool notify, CCError **error) {
        return CCVirtualGoodsStorage::getInstance()->setBalance(this, balance, notify, error);
    }
    
    int CCVirtualGood::getBalance(CCError **error) {
        return CCVirtualGoodsStorage::getInstance()->getBalance(this, error);
    }
}