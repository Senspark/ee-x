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

#include "CCEquippableVG.h"
#include "CCJsonHelper.h"
#include "CCSoomlaUtils.h"
#include "CCStoreInfo.h"
#include "CCVirtualCategory.h"
#include "CCVirtualGoodsStorage.h"

#define EQUIPPING_MODEL_LOCAL "local"
#define EQUIPPING_MODEL_CATEGORY "category"
#define EQUIPPING_MODEL_GLOBAL "global"

namespace soomla {

USING_NS_CC;

#define TAG "SOOMLA EquippableVG"

CCEquippableVG* CCEquippableVG::create(std::int32_t equippingModel,
                                       const std::string& name,
                                       const std::string& description,
                                       const std::string& itemId,
                                       CCPurchaseType* purchaseType) {
    CCEquippableVG* ret = new CCEquippableVG();
    if (ret->init(equippingModel, name, description, itemId, purchaseType)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CCEquippableVG::init(std::int32_t equippingModel, const std::string& name,
                          const std::string& description,
                          const std::string& itemId,
                          soomla::CCPurchaseType* purchaseType) {
    bool res = CCLifetimeVG::init(name, description, itemId, purchaseType);
    if (res) {
        setEquippingModel(equippingModel);
        return true;
    } else {
        return false;
    }
}

bool CCEquippableVG::initWithValueMap(const cocos2d::ValueMap& map) {
    bool ret = CCLifetimeVG::initWithValueMap(map);
    if (ret) {
        fillEquippingModelFromValueMap(map);
        return true;
    } else {
        return false;
    }
}

ValueMap CCEquippableVG::toValueMap() {
    ValueMap map = CCPurchasableVirtualItem::toValueMap();

    putEquippingModelToValueMap(map);

    return map;
}

const char* CCEquippableVG::getType() const {
    return CCStoreConsts::JSON_JSON_TYPE_EQUIPPABLE_VG;
}

void CCEquippableVG::fillEquippingModelFromValueMap(
    const cocos2d::ValueMap& map) {
    CCASSERT(map.count(CCStoreConsts::JSON_EQUIPPABLE_EQUIPPING),
             "invalid object type in dictionary");
    const auto& equippingModelStr =
        map.at(CCStoreConsts::JSON_EQUIPPABLE_EQUIPPING).asString();

    if (equippingModelStr.compare(EQUIPPING_MODEL_LOCAL) == 0) {
        setEquippingModel(kLocal);
    } else if (equippingModelStr.compare(EQUIPPING_MODEL_CATEGORY) == 0) {
        setEquippingModel(kCategory);
    } else if (equippingModelStr.compare(EQUIPPING_MODEL_GLOBAL) == 0) {
        setEquippingModel(kGlobal);
    } else {
        CC_ASSERT(false);
    }
}

void CCEquippableVG::putEquippingModelToValueMap(cocos2d::ValueMap& map) {
    EquippingModel equippingModel = (EquippingModel)getEquippingModel();
    std::string strEquippingModel = "";
    switch (equippingModel) {
    case kLocal: {
        strEquippingModel = EQUIPPING_MODEL_LOCAL;
        break;
    }
    case kCategory: {
        strEquippingModel = EQUIPPING_MODEL_CATEGORY;
        break;
    }
    case kGlobal: {
        strEquippingModel = EQUIPPING_MODEL_GLOBAL;
        break;
    }
    default: {
        CC_ASSERT(false);
        strEquippingModel = "ERROR";
    }
    }
    map[CCStoreConsts::JSON_EQUIPPABLE_EQUIPPING] = strEquippingModel;
}

void CCEquippableVG::equip(bool notify, CCError** error) {
    // only if the user has bought this EquippableVG, the EquippableVG is
    // equipped.
    if (CCVirtualGoodsStorage::getInstance()->getBalance(this) > 0) {
        EquippingModel equippingModel = (EquippingModel)getEquippingModel();
        if (equippingModel == kCategory) {
            const char* itemId = getItemId().c_str();
            CCVirtualCategory* category =
                CCStoreInfo::sharedStoreInfo()->getCategoryForVirtualGood(
                    itemId, error);
            if (category == nullptr) {
                CCSoomlaUtils::logError(
                    TAG, StringUtils::format(
                             "Tried to unequip all other category VirtualGoods "
                             "but there was no associated category. virtual "
                             "good itemId: %s",
                             itemId)
                             .c_str());
                return;
            }

            auto& goodItemIds = category->getGoodItemIds();
            for (auto goodItemIdStr : goodItemIds) {
                const char* goodItemId = goodItemIdStr.c_str();
                CCEquippableVG* equippableVG = dynamic_cast<CCEquippableVG*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(goodItemId,
                                                                    error));
                if (equippableVG == nullptr) {
                    CCSoomlaUtils::logError(
                        TAG,
                        StringUtils::format(
                            "On equip, couldn't find one of the itemIds in the "
                            "category. Continuing to the next one. itemId: %s",
                            goodItemId)
                            .c_str());
                } else if (equippableVG != this) {
                    equippableVG->unequip(notify, error);
                }
            }
        } else if (equippingModel == kGlobal) {
            auto& virtualGoods = CCStoreInfo::sharedStoreInfo()->getGoods();
            for (auto good : virtualGoods) {
                auto equippableVG = dynamic_cast<CCEquippableVG*>(good);
                if ((good != this) && (equippableVG != nullptr)) {
                    equippableVG->unequip(notify, error);
                }
            }
        }

        CCVirtualGoodsStorage::getInstance()->equip(this, notify, error);
    } else {
        auto&& errorValue = Value(StringUtils::format(
            "You tried to equip virtual good with itemId: %s \
                                                            but you don't have any of it.",
            getItemId().c_str()));
        CCError::tryFillError(error, errorValue, TAG);
    }
}

void CCEquippableVG::unequip(bool notify, CCError** error) {
    CCVirtualGoodsStorage::getInstance()->unequip(this, notify, error);
}
} // namespace soomla
