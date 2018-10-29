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

#include "CCReward.h"
#include "CCRewardStorage.h"
#include "CCSoomlaUtils.h"

#define TAG "SOOMLA Reward"

cocos2d::Map<std::string, soomla::CCReward*> soomla::CCReward::rewardsMap;

bool soomla::CCReward::init(const std::string& id, const std::string& name,
                            CCSchedule* schedule) {
    bool ret = CCSoomlaEntity::init(id, name);

    if (ret) {
        setSchedule(schedule != nullptr ? schedule
                                     : CCSchedule::createAnyTimeOnce());
        addReward(this);
        return true;
    }

    return ret;
}

bool soomla::CCReward::initWithValueMap(const cocos2d::ValueMap& map) {
    bool ret = CCSoomlaEntity::initWithValueMap(map);

    if (ret) {
        CCSchedule* schedule;
        if (map.count(CCCoreConsts::JSON_SCHEDULE)) {
            schedule = CCSchedule::createWithValueMap(
                map.at(CCCoreConsts::JSON_SCHEDULE).asValueMap());
        } else {
            schedule = CCSchedule::createAnyTimeOnce();
        }
        setSchedule(schedule);
        addReward(this);
        return true;
    }

    return false;
}

cocos2d::ValueMap soomla::CCReward::toValueMap() {
    auto&& map = CCSoomlaEntity::toValueMap();
    if (mSchedule) {
        map[CCCoreConsts::JSON_SCHEDULE] = mSchedule->toValueMap();
    }
    return map;
}

soomla::CCReward::~CCReward() {
    CC_SAFE_RELEASE(mSchedule);
}

bool soomla::CCReward::take() {
    if (!CCRewardStorage::getInstance()->isRewardGiven(this)) {
        CCSoomlaUtils::logDebug(
            TAG, cocos2d::StringUtils::format("Reward not given. id: %s",
                                              getId().c_str())
                     .c_str());
        return false;
    }

    if (takeInner()) {
        CCRewardStorage::getInstance()->setRewardStatus(this, false);
        return true;
    }

    return false;
}

bool soomla::CCReward::give() {
    if (!mSchedule->approve(
            CCRewardStorage::getInstance()->getTimesGiven(this))) {
        CCSoomlaUtils::logDebug(
            TAG, cocos2d::StringUtils::format(
                     "(Give) Reward is not approved by Schedule. id: %s",
                     getId().c_str())
                     .c_str());
        return false;
    }

    if (giveInner()) {
        CCRewardStorage::getInstance()->setRewardStatus(this, true);
        return true;
    }

    return false;
}

bool soomla::CCReward::isOwned() {
    return CCRewardStorage::getInstance()->isRewardGiven(this);
}

void soomla::CCReward::addReward(CCReward* reward) {
    rewardsMap.insert(reward->getId(), reward);
}

soomla::CCReward* soomla::CCReward::getReward(const std::string& id) {
    return rewardsMap.at(id);
}
