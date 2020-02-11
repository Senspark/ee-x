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

#include "soomla/rewards/CCBadgeReward.h"

#include "soomla/CCJsonHelper.h"

soomla::CCBadgeReward*
soomla::CCBadgeReward::create(const std::string& id, const std::string& name,
                              const std::string& iconUrl) {
    CCBadgeReward* ret = new CCBadgeReward();
    if (ret->init(id, name, iconUrl)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool soomla::CCBadgeReward::init(const std::string& id, const std::string& name,
                                 const std::string& iconUrl) {
    bool result = CCReward::init(id, name, nullptr);

    if (result) {
        setIconUrl(iconUrl);
        return true;
    } else {
        return false;
    }
}

bool soomla::CCBadgeReward::initWithValueMap(const cocos2d::ValueMap& map) {
    CCReward::initWithValueMap(map);
    fillIconUrlFromValueMap(map);
    return true;
}

cocos2d::ValueMap soomla::CCBadgeReward::toValueMap() {
    auto&& map = CCReward::toValueMap();
    putIconUrlToValueMap(map);
    return map;
}

const char* soomla::CCBadgeReward::getType() const {
    return CCCoreConsts::JSON_JSON_TYPE_BADGE;
}

bool soomla::CCBadgeReward::takeInner() {
    // nothing to do here... the parent Reward takes in storage
    return true;
}

bool soomla::CCBadgeReward::giveInner() {
    // nothing to do here... the parent Reward gives in storage
    return true;
}

void soomla::CCBadgeReward::fillIconUrlFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCCoreConsts::JSON_ICON_URL)) {
        setIconUrl(map.at(CCCoreConsts::JSON_ICON_URL).asString());
    } else {
        setIconUrl("");
    }
}

void soomla::CCBadgeReward::putIconUrlToValueMap(cocos2d::ValueMap& map) {
    map[CCCoreConsts::JSON_ICON_URL] = getIconUrl();
}
