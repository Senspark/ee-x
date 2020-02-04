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

#ifndef __CCSchedule_H_
#define __CCSchedule_H_

#include "Soomla/CCCoreConsts.h"
#include "Soomla/CCSoomlaMacros.h"
#include "Soomla/domain/CCDomain.h"

namespace soomla {
class CCSchedule : public CCDomain {
public:
    enum Recurrence { EVERY_MONTH, EVERY_WEEK, EVERY_DAY, EVERY_HOUR, NONE };

    class CCDateTimeRange : public CCDomain {
    public:
        timeval mStart;
        timeval mEnd;

        static CCDateTimeRange* create(timeval start, timeval end);
        SL_CREATE_WITH_VALUE_MAP(CCDateTimeRange);
        virtual bool init(timeval start, timeval end);
        virtual bool initWithValueMap(const cocos2d::ValueMap& dict) override;
        virtual ~CCDateTimeRange() = default;
        virtual cocos2d::ValueMap toValueMap() override;
    };

    CCSchedule()
        : CCDomain()
        , mRequiredRecurrence(Recurrence::NONE)
        , mActivationLimit(0) {}

    static CCSchedule*
    create(std::int32_t requiredRecurrence,
           const cocos2d::Vector<CCDateTimeRange*>& timeRanges,
           std::int32_t activationLimit);
    static CCSchedule* create(std::int32_t activationLimit);
    static CCSchedule* create(std::int32_t requiredRecurrence, timeval start,
                              timeval end, std::int32_t activationLimit);

    static CCSchedule* createAnyTimeOnce();
    static CCSchedule* createAnyTimeLimited(int activationLimit);
    static CCSchedule* createAnyTimeUnLimited();

    SL_CREATE_WITH_VALUE_MAP(CCSchedule);

    virtual bool init(std::int32_t requiredRecurrence,
                      const cocos2d::Vector<CCDateTimeRange*>& timeRanges,
                      std::int32_t activationLimit);
    virtual bool initWithValueMap(const cocos2d::ValueMap& dict) override;

    virtual ~CCSchedule() = default;

    virtual cocos2d::ValueMap toValueMap() override;

    bool approve(int activationTimes);

    CC_SYNTHESIZE(std::int32_t, mRequiredRecurrence, RequiredRecurrence);
    CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vector<CCDateTimeRange*>, mTimeRanges,
                              TimeRanges);
    CC_SYNTHESIZE(std::int32_t, mActivationLimit, ActivationLimit)
protected:
    void fillRequiredRecurrenceFromValueMap(const cocos2d::ValueMap& map);
    void putRequiredRecurrenceToValueMap(cocos2d::ValueMap& map);
    void fillTimeRangesFromValueMap(const cocos2d::ValueMap& map);
    void putTimeRangesToValueMap(cocos2d::ValueMap& map);
    void fillActivationLimitFromValueMap(const cocos2d::ValueMap& map);
    void putActivationLimitToValueMap(cocos2d::ValueMap& map);
};
} // namespace soomla

#endif //__CCSchedule_H_
