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

#ifndef __CCReward_H_
#define __CCReward_H_

#include <cocos2d.h>

#include "Soomla/CCCoreConsts.h"
#include "Soomla/CCSchedule.h"
#include "Soomla/CCSoomlaMacros.h"
#include "Soomla/domain/CCDomain.h"
#include "Soomla/domain/CCSoomlaEntity.h"

namespace soomla {
class CCReward : public CCSoomlaEntity {
public:
    CCReward()
        : CCSoomlaEntity()
        , mSchedule(nullptr){};

    using CCSoomlaEntity::init;

    virtual bool init(const std::string& id, const std::string& name,
                      CCSchedule* schedule = nullptr);

    virtual bool initWithValueMap(const cocos2d::ValueMap& map);

    virtual cocos2d::ValueMap toValueMap();

    virtual ~CCReward();
    
    CC_SYNTHESIZE_RETAIN(CCSchedule*, mSchedule, Schedule);

public:
    virtual bool take();
    virtual bool give();
    bool isOwned();

    static void addReward(CCReward* reward);
    static CCReward* getReward(const std::string& id);
protected:
    virtual bool takeInner() = 0;
    virtual bool giveInner() = 0;

private:
    static cocos2d::Map<std::string, CCReward*> rewardsMap;
};
} // namespace soomla

#endif //__CCReward_H_
