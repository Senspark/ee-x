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

#ifndef __CCAggregateReward_H_
#define __CCAggregateReward_H_

#include "cocos2d.h"
#include "CCSoomlaMacros.h"
#include "CCCoreConsts.h"
#include "CCReward.h"

namespace soomla {
    class CCAggregateReward : public CCReward {
    public:
        CCAggregateReward() : CCReward() {
        };

        virtual bool init(const std::string& id, const std::string& name,
                          const cocos2d::Vector<CCDomain*>& rewards,
                          CCSchedule* schedule = nullptr);

        virtual bool initWithValueMap(const cocos2d::ValueMap& map);

        virtual cocos2d::ValueMap toValueMap();
        
        virtual ~CCAggregateReward();

        CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vector<CCDomain*>, mRewards,
                                  Rewards);

    protected:
    };
}

#endif //__CCAggregateReward_H_
