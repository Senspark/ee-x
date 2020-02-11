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

#ifndef __CCRandomReward_H_
#define __CCRandomReward_H_

#include <cocos2d.h>

#include "soomla/CCSoomlaMacros.h"
#include "soomla/rewards/CCAggregateReward.h"

namespace soomla {
class CCRandomReward : public CCAggregateReward {
    CC_SYNTHESIZE_RETAIN(CCReward*, mLastGivenReward, LastGivenReward);

public:
    CCRandomReward()
        : CCAggregateReward()
        , mLastGivenReward(nullptr){};

    static CCRandomReward* create(const std::string& id,
                                  const std::string& name,
                                  const cocos2d::Vector<CCDomain*>& rewards,
                                  CCSchedule* schedule);

    SL_CREATE_WITH_VALUE_MAP(CCRandomReward);

    virtual ~CCRandomReward();

    virtual const char* getType() const override;

protected:
    virtual bool takeInner() override;

    virtual bool giveInner() override;
};
} // namespace soomla

#endif //__CCRandomReward_H_
