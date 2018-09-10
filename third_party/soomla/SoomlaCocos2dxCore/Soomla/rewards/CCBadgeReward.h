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

#ifndef __CCBadgeReward_H_
#define __CCBadgeReward_H_

#include "CCReward.h"
#include "CCSoomlaMacros.h"
#include "cocos2d.h"

namespace soomla {
class CCBadgeReward : public CCReward {
public:
    CCBadgeReward()
        : CCReward()
        , mIconUrl(""){};

    static CCBadgeReward* create(const std::string& id, const std::string& name,
                                 const std::string& iconUrl = "");

    SL_CREATE_WITH_VALUE_MAP(CCBadgeReward);

    using CCReward::init;

    virtual bool init(const std::string& id, const std::string& name,
                      const std::string& iconUrl = "") override;

    virtual bool initWithValueMap(const cocos2d::ValueMap& map) override;

    virtual cocos2d::ValueMap toValueMap() override;
    virtual ~CCBadgeReward() = default;

    virtual const char* getType() const override;
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mIconUrl, IconUrl);

protected:
    virtual bool takeInner() override;

    virtual bool giveInner() override;
    
    void fillIconUrlFromValueMap(const cocos2d::ValueMap& map);
    void putIconUrlToValueMap(cocos2d::ValueMap& map);
};
} // namespace soomla

#endif //__CCBadgeReward_H_
