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

#include "CCAggregateReward.h"
#include "CCJsonHelper.h"

bool soomla::CCAggregateReward::init(const std::string& id,
                                     const std::string& name,
                                     const cocos2d::Vector<CCDomain*>& rewards,
                                     CCSchedule* schedule) {
    bool result = CCReward::init(id, name, schedule);
    
    if (result) {
        setRewards(rewards);
        return true;
    } else {
        return false;
    }
}

bool soomla::CCAggregateReward::initWithValueMap(const cocos2d::ValueMap& map) {
    CCReward::initWithValueMap(map);
    
    CC_ASSERT(map.count(CCCoreConsts::JSON_REWARDS));
    auto& vRewards = map.at(CCCoreConsts::JSON_REWARDS).asValueVector();
    setRewards(getDomainsFromDictArray(vRewards));
    return true;
}

cocos2d::ValueMap soomla::CCAggregateReward::toValueMap() {
    auto&& map = CCReward::toValueMap();
    
    auto&& vRewards = this->getDictArrayFromDomains(getRewards());
    map[CCCoreConsts::JSON_REWARDS] = vRewards;
    
    return map;
}

soomla::CCAggregateReward::~CCAggregateReward() {
    mRewards.clear();
}
