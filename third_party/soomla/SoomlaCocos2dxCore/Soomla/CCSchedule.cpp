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

#include "Soomla/CCSchedule.h"

#include "Soomla/CCJsonHelper.h"
#include "Soomla/CCSoomlaUtils.h"

#define TAG "SOOMLA Schedule"

namespace soomla {
CCSchedule::CCDateTimeRange* CCSchedule::CCDateTimeRange::create(timeval start,
                                                                 timeval end) {
    CCDateTimeRange* ret = new CCDateTimeRange();
    if (ret->init(start, end)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CCSchedule::CCDateTimeRange::init(timeval start, timeval end) {
    mStart = start;
    mEnd = end;

    return true;
}

bool CCSchedule::CCDateTimeRange::initWithValueMap(
    const cocos2d::ValueMap& dict) {
    time_t seconds;

    CC_ASSERT(dict.count(CCCoreConsts::JSON_SCHE_RANGE_START) &&
              dict.at(CCCoreConsts::JSON_SCHE_RANGE_START).getType() ==
                  cocos2d::Value::Type::DOUBLE);
    auto startMs = dict.at(CCCoreConsts::JSON_SCHE_RANGE_START).asDouble();

    // convert ms to timeval
    seconds = (time_t)(startMs / 1000);
    this->mStart.tv_sec = seconds;
    this->mStart.tv_usec = (suseconds_t)((startMs - 1000 * seconds) * 1000);

    CC_ASSERT(dict.count(CCCoreConsts::JSON_SCHE_RANGE_END) &&
              (dict.at(CCCoreConsts::JSON_SCHE_RANGE_END).getType() ==
               cocos2d::Value::Type::DOUBLE));
    auto endMs = dict.at(CCCoreConsts::JSON_SCHE_RANGE_END).asDouble();

    // convert ms to timeval
    seconds = (time_t)(endMs / 1000);
    this->mStart.tv_sec = seconds;
    this->mStart.tv_usec = (suseconds_t)((endMs - 1000 * seconds) * 1000);

    return true;
}

cocos2d::ValueMap CCSchedule::CCDateTimeRange::toValueMap() {
    cocos2d::ValueMap map;

    double ms = 1000 * mStart.tv_sec + mStart.tv_usec / 1000.0f;
    map[CCCoreConsts::JSON_SCHE_RANGE_START] = ms;

    ms = 1000 * mEnd.tv_sec + mEnd.tv_usec / 1000.0f;
    map[CCCoreConsts::JSON_SCHE_RANGE_END] = ms;

    return this->putTypeData(map, CCCoreConsts::JSON_JSON_TYPE_DATE_TIME_RANGE);
}

CCSchedule*
CCSchedule::create(std::int32_t requiredRecurrence,
                   const cocos2d::Vector<CCDateTimeRange*>& timeRanges,
                   std::int32_t activationLimit) {
    CCSchedule* ret = new CCSchedule();
    if (ret->init(requiredRecurrence, timeRanges, activationLimit)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

CCSchedule* CCSchedule::create(std::int32_t activationLimit) {
    return CCSchedule::create(CCSchedule::Recurrence::NONE,
                              cocos2d::Vector<CCDateTimeRange*>(),
                              activationLimit);
}

CCSchedule* CCSchedule::create(std::int32_t requiredRecurrence, timeval start,
                               timeval end, std::int32_t activationLimit) {
    cocos2d::Vector<CCDateTimeRange*> timeRanges;
    timeRanges.pushBack(CCDateTimeRange::create(start, end));
    return create(requiredRecurrence, timeRanges, activationLimit);
}

CCSchedule* CCSchedule::createAnyTimeOnce() {
    return CCSchedule::create(1);
}

CCSchedule* CCSchedule::createAnyTimeLimited(int activationLimit) {
    return CCSchedule::create(activationLimit);
}

CCSchedule* CCSchedule::createAnyTimeUnLimited() {
    return CCSchedule::create(0);
}

bool CCSchedule::init(std::int32_t requiredRecurrence,
                      const cocos2d::Vector<CCDateTimeRange*>& timeRanges,
                      std::int32_t activationLimit) {
    setRequiredRecurrence(requiredRecurrence);
    setTimeRanges(timeRanges);
    setActivationLimit(activationLimit);

    return true;
}

bool CCSchedule::initWithValueMap(const cocos2d::ValueMap& map) {
    fillRequiredRecurrenceFromValueMap(map);
    fillTimeRangesFromValueMap(map);
    fillActivationLimitFromValueMap(map);

    return true;
}

cocos2d::ValueMap CCSchedule::toValueMap() {
    cocos2d::ValueMap map;

    putRequiredRecurrenceToValueMap(map);
    putTimeRangesToValueMap(map);
    putActivationLimitToValueMap(map);

    return putTypeData(map, CCCoreConsts::JSON_JSON_TYPE_SCHEDULE);
}

bool CCSchedule::approve(int activationTimes) {
    struct timeval now;
    gettimeofday(&now, nullptr);

    if (mActivationLimit && mActivationLimit < 1 && mTimeRanges.size() == 0) {
        CCSoomlaUtils::logDebug(
            TAG, "There's no activation limit and no TimeRanges. APPROVED!");
        return true;
    }

    if (mActivationLimit > 0 && activationTimes >= mActivationLimit) {
        CCSoomlaUtils::logDebug(TAG, "Activation limit exceeded.");
        return false;
    }

    if (mTimeRanges.size() == 0) {
        CCSoomlaUtils::logDebug(
            TAG, "We have an activation limit that was not reached. Also, we "
                 "don't have any time ranges. APPROVED!");
        return true;
    }

    // NOTE: From this point on ... we know that we didn't reach the activation
    // limit AND we have TimeRanges.
    //         We'll just make sure the time ranges and the Recurrence copmlies.

    CCDateTimeRange* dateTimeRange;
    for (auto ref : mTimeRanges) {
        dateTimeRange = dynamic_cast<CCDateTimeRange*>(ref);
        CC_ASSERT(dateTimeRange);
        if (!timercmp(&now, &dateTimeRange->mStart, <) &&
            !timercmp(&now, &dateTimeRange->mEnd, >)) {
            CCSoomlaUtils::logDebug(
                TAG, "We are just in one of the time spans, it can't get any "
                     "better then that. APPROVED!");
            return true;
        }
    }

    // we don't need to continue if RequiredRecurrence is NONE
    if (mRequiredRecurrence == Recurrence::NONE) {
        return false;
    }

    tm* ptm;

    ptm = localtime(&now.tv_sec);
    tm nowTm = *ptm;

    for (auto ref : mTimeRanges) {
        dateTimeRange = dynamic_cast<CCDateTimeRange*>(ref);

        ptm = localtime(&dateTimeRange->mStart.tv_sec);
        tm begTm = *ptm;

        ptm = localtime(&dateTimeRange->mEnd.tv_sec);
        tm endTm = *ptm;

        if (nowTm.tm_min >= begTm.tm_min && nowTm.tm_min <= endTm.tm_min) {
            CCSoomlaUtils::logDebug(
                TAG, "Now is in one of the time ranges' minutes span.");

            if (mRequiredRecurrence == Recurrence::EVERY_HOUR) {
                CCSoomlaUtils::logDebug(
                    TAG, "It's a EVERY_HOUR recurrence. APPROVED!");
                return true;
            }

            if (nowTm.tm_hour >= begTm.tm_hour &&
                nowTm.tm_hour <= endTm.tm_hour) {
                CCSoomlaUtils::logDebug(
                    TAG, "Now is in one of the time ranges' hours span.");

                if (mRequiredRecurrence == Recurrence::EVERY_DAY) {
                    CCSoomlaUtils::logDebug(
                        TAG, "It's a EVERY_DAY recurrence. APPROVED!");
                    return true;
                }

                if (nowTm.tm_wday >= begTm.tm_wday &&
                    nowTm.tm_wday <= endTm.tm_wday) {
                    CCSoomlaUtils::logDebug(
                        TAG,
                        "Now is in one of the time ranges' day-of-week span.");

                    if (mRequiredRecurrence == Recurrence::EVERY_WEEK) {
                        CCSoomlaUtils::logDebug(
                            TAG, "It's a EVERY_WEEK recurrence. APPROVED!");
                        return true;
                    }

                    if (nowTm.tm_mday >= begTm.tm_mday &&
                        nowTm.tm_mday <= endTm.tm_mday) {
                        CCSoomlaUtils::logDebug(
                            TAG,
                            "Now is in one of the time ranges' days span.");

                        if (mRequiredRecurrence == Recurrence::EVERY_MONTH) {
                            CCSoomlaUtils::logDebug(
                                TAG,
                                "It's a EVERY_MONTH recurrence. APPROVED!");
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

void CCSchedule::fillRequiredRecurrenceFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCCoreConsts::JSON_SCHE_REC)) {
        setRequiredRecurrence(map.at(CCCoreConsts::JSON_SCHE_REC).asInt());
    } else {
        setRequiredRecurrence(0);
    }
}

void CCSchedule::putRequiredRecurrenceToValueMap(cocos2d::ValueMap& map) {
    map[CCCoreConsts::JSON_SCHE_REC] = getRequiredRecurrence();
}

void CCSchedule::fillTimeRangesFromValueMap(const cocos2d::ValueMap& map) {
    cocos2d::Vector<CCDateTimeRange*> timeRanges;
    
    if (map.count(CCCoreConsts::JSON_SCHE_RANGES)) {
        auto& vvTimeRanges =
            map.at(CCCoreConsts::JSON_SCHE_RANGES).asValueVector();
        for (auto& vmTimeRange : vvTimeRanges) {
            auto timeRange = CCDateTimeRange::createWithValueMap(vmTimeRange.asValueMap());
            timeRanges.pushBack(timeRange);
        }
    }
    setTimeRanges(timeRanges);
}

void CCSchedule::putTimeRangesToValueMap(cocos2d::ValueMap& map) {
    cocos2d::ValueVector vvTimeRanges;
    for (auto timeRange : mTimeRanges) {
        vvTimeRanges.push_back(cocos2d::Value(timeRange->toValueMap()));
    }
    map[CCCoreConsts::JSON_SCHE_RANGES] = vvTimeRanges;
}

void CCSchedule::fillActivationLimitFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCCoreConsts::JSON_SCHE_APPROVALS)) {
        setActivationLimit(map.at(CCCoreConsts::JSON_SCHE_APPROVALS).asInt());
    } else {
        setActivationLimit(0);
    }
}

void CCSchedule::putActivationLimitToValueMap(cocos2d::ValueMap& map) {
    map[CCCoreConsts::JSON_SCHE_APPROVALS] = mActivationLimit;
}
} // namespace soomla
