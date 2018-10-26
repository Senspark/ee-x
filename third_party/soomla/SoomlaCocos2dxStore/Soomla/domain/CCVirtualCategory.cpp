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

#include "CCJsonHelper.h"
#include "CCVirtualCategory.h"

USING_NS_CC;

namespace soomla {

CCVirtualCategory*
CCVirtualCategory::create(const std::string& name,
                          const std::vector<std::string>& goodItemIds) {
    CCVirtualCategory* ret = new CCVirtualCategory();
    if (ret->init(name, goodItemIds)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool CCVirtualCategory::init(const std::string& name,
                             const std::vector<std::string>& goodItemIds) {
    setName(name);
    setGoodItemIds(goodItemIds);
    return true;
}

bool CCVirtualCategory::initWithValueMap(const cocos2d::ValueMap& map) {
    fillNameFromValueMap(map);
    fillGoodItemIdsFromValueMap(map);

    return true;
}

ValueMap CCVirtualCategory::toValueMap() {
    ValueMap map;

    putNameToValueMap(map);
    putGoodItemIdsToValueMap(map);

    return this->putTypeData(map,
                             CCStoreConsts::JSON_JSON_TYPE_VIRTUAL_CATEGORY);
}

void CCVirtualCategory::fillNameFromValueMap(const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_CATEGORY_NAME)) {
        setName(map.at(CCStoreConsts::JSON_CATEGORY_NAME).asString());
    } else {
        setName("");
    }
}

void CCVirtualCategory::putNameToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_CATEGORY_NAME] = getName();
}

void CCVirtualCategory::fillGoodItemIdsFromValueMap(
    const cocos2d::ValueMap& map) {
    mGoodItemIds.clear();
    if (map.count(CCStoreConsts::JSON_CATEGORY_GOODS_ITEM_IDS)) {
        auto& vGoodItemIds =
            map.at(CCStoreConsts::JSON_CATEGORY_GOODS_ITEM_IDS).asValueVector();
        for (auto id : vGoodItemIds) {
            mGoodItemIds.push_back(id.asString());
        }
    }
}

void CCVirtualCategory::putGoodItemIdsToValueMap(cocos2d::ValueMap& map) {
    ValueVector vIds;
    for (auto id : mGoodItemIds) {
        vIds.push_back(Value(id));
    }
    map[CCStoreConsts::JSON_CATEGORY_GOODS_ITEM_IDS] = vIds;
}
} // namespace soomla
