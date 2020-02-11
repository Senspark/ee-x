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

#include "soomla/domain/virtualGoods/CCSingleUseVG.h"

#include "soomla/data/CCVirtualGoodsStorage.h"

namespace soomla {
USING_NS_CC;

CCSingleUseVG* CCSingleUseVG::create(const std::string& name, const std::string& description,
                                     const std::string& itemId,
                                     CCPurchaseType* purchaseType) {
    CCSingleUseVG* ret = new CCSingleUseVG();
    if (ret->init(name, description, itemId, purchaseType)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CCSingleUseVG::canBuy() {
    return true;
}

int CCSingleUseVG::give(int amount, bool notify, CCError** error) {
    return CCVirtualGoodsStorage::getInstance()->add(this, amount, notify,
                                                     error);
}

int CCSingleUseVG::take(int amount, bool notify, CCError** error) {
    return CCVirtualGoodsStorage::getInstance()->remove(this, amount, notify,
                                                        error);
}

const char* CCSingleUseVG::getType() const {
    return CCStoreConsts::JSON_JSON_TYPE_SINGLE_USE_VG;
}
} // namespace soomla
