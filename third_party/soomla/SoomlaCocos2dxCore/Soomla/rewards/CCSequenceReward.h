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

#ifndef __CCSequenceReward_H_
#define __CCSequenceReward_H_

#include <cocos2d.h>

#include "Soomla/CCSoomlaMacros.h"
#include "Soomla/rewards/CCAggregateReward.h"
#include "Soomla/rewards/CCReward.h"

namespace soomla {
class CCSequenceReward : public CCAggregateReward {
public:
    CCSequenceReward()
        : CCAggregateReward(){};

    static CCSequenceReward* create(const std::string& id,
                                    const std::string& name,
                                    const cocos2d::Vector<CCDomain*>& rewards);

    SL_CREATE_WITH_VALUE_MAP(CCSequenceReward);

    virtual ~CCSequenceReward();

    virtual const char* getType() const override;

    CCReward* getLastGivenReward();
    bool hasMoreToGive();
    bool forceNextRewardToGive(CCReward* reward);

protected:
    virtual bool takeInner() override;

    virtual bool giveInner() override;
};
} // namespace soomla

#endif //__CCSequenceReward_H_
