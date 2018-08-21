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

#include "CCDomainFactory.h"
#include "CCJsonHelper.h"
#include "CCSoomlaEntity.h"
#include "CCSoomlaUtils.h"

namespace soomla {

#define TAG "SOOMLA SoomlaEntity"

using namespace cocos2d;

bool CCSoomlaEntity::init(const std::string& id, const std::string& name,
                          const std::string& description) {
    setId(id);
    setName(name);
    setDescription(description);

    return true;
}

bool CCSoomlaEntity::initWithValueMap(const cocos2d::ValueMap& dict) {
    fillIdFromValueMap(dict);
    fillNameFromValueMap(dict);
    fillDescriptionFromValueMap(dict);
    
    if (mId.length() == 0) {
        CCSoomlaUtils::logError(
            TAG,
            "This is BAD! We don't have ID in the given dict. Stopping here.");
        CC_ASSERT(false);
    }
    return true;
}

cocos2d::ValueMap CCSoomlaEntity::toValueMap() {
    if (mId.length() == 0) {
        CCSoomlaUtils::logError(TAG, "This is BAD! We don't have ID in the "
                                     "this SoomlaEntity. Stopping here.");
        CC_ASSERT(false);
        return ValueMapNull;
    }
    ValueMap map;
    putIdToValueMap(map);
    putNameToValueMap(map);
    putDescriptionToValueMap(map);
    return map;
}

bool CCSoomlaEntity::equals(cocos2d::Ref* obj) const {
    // If parameter is null return false.
    if (obj == NULL) {
        return false;
    }

    CCSoomlaEntity* castedObj = dynamic_cast<CCSoomlaEntity*>(obj);
    if (!castedObj) {
        return false;
    }

    if (strcmp(castedObj->getType(), getType()) != 0) {
        return false;
    }

    if (castedObj->getId().compare(getId()) != 0) {
        return false;
    }

    return true;
}

CCSoomlaEntity* CCSoomlaEntity::clone(const std::string& newId) {
    ValueMap&& map = toValueMap();
    map[CCCoreConsts::JSON_ITEM_ITEM_ID] = newId;
    return (CCSoomlaEntity*)CCDomainFactory::getInstance()
        ->createWithValueMapAndType(map, getType());
}

void CCSoomlaEntity::fillIdFromValueMap(const cocos2d::ValueMap& dict) {
    if (dict.count(CCCoreConsts::JSON_ITEM_ITEM_ID)) {
        setId(dict.at(CCCoreConsts::JSON_ITEM_ITEM_ID).asString());
    } else {
        setId("");
    }
}

void CCSoomlaEntity::putIdToValueMap(cocos2d::ValueMap& dict) {
    dict[CCCoreConsts::JSON_ITEM_ITEM_ID] = getId();
}

void CCSoomlaEntity::fillNameFromValueMap(const cocos2d::ValueMap& dict) {
    if (dict.count(CCCoreConsts::JSON_ITEM_NAME)) {
        setName(dict.at(CCCoreConsts::JSON_ITEM_NAME).asString());
    } else {
        setName("");
    }
}

void CCSoomlaEntity::putNameToValueMap(cocos2d::ValueMap& dict) {
    dict[CCCoreConsts::JSON_ITEM_NAME] = getName();
}

void CCSoomlaEntity::fillDescriptionFromValueMap(
    const cocos2d::ValueMap& dict) {
    if (dict.count(CCCoreConsts::JSON_ITEM_DESCRIPTION)) {
        setDescription(dict.at(CCCoreConsts::JSON_ITEM_DESCRIPTION).asString());
    } else {
        setDescription("");
    }
}

void CCSoomlaEntity::putDescriptionToValueMap(cocos2d::ValueMap& dict) {
    dict[CCCoreConsts::JSON_ITEM_DESCRIPTION] = getDescription();
}

} // namespace soomla
