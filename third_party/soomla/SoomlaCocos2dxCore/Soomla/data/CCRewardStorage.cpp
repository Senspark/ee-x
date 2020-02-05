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

#include "Soomla/data/CCRewardStorage.h"

#include "Soomla/CCCoreEventDispatcher.h"
#include "Soomla/data/CCKeyValueStorage.h"
#include "Soomla/NativeImpl/CCNativeRewardStorage.h"
#include "Soomla/rewards/CCReward.h"
#include "Soomla/rewards/CCSequenceReward.h"

#define TAG "SOOMLA RewardStorage"

namespace soomla {

USING_NS_CC;

static CCRewardStorage* s_SharedRewardStorage = nullptr;

CCRewardStorage* CCRewardStorage::getInstance() {
    if (!s_SharedRewardStorage) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) ||                                 \
    (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        s_SharedRewardStorage = new CCNativeRewardStorage();
#else
        s_SharedRewardStorage = new CCRewardStorage();
#endif
        s_SharedRewardStorage->retain();
    }

    return s_SharedRewardStorage;
}

bool CCRewardStorage::isRewardGiven(CCReward* reward) {
    return getTimesGiven(reward) > 0;
}

void CCRewardStorage::setRewardStatus(CCReward* reward, bool give) {
    setRewardStatus(reward, give, true);
}

void CCRewardStorage::setRewardStatus(CCReward* reward, bool give,
                                      bool notify) {
    this->setTimesGiven(reward, give, notify);
}

int CCRewardStorage::getTimesGiven(CCReward* reward) {
    auto&& key = keyRewardTimesGiven(reward->getId());
    const auto& val = CCKeyValueStorage::getInstance()->getValue(key);
    if (val.length() == 0) {
        return 0;
    }
    return std::stoi(val);
}

int CCRewardStorage::getLastSeqIdxGiven(CCSequenceReward* sequenceReward) {
    auto&& key = keyRewardIdxSeqGiven(sequenceReward->getId());
    const auto& val = CCKeyValueStorage::getInstance()->getValue(key);
    if (val.length() == 0) {
        return -1;
    }
    return std::stoi(val);
}

void CCRewardStorage::setLastSeqIdxGiven(CCSequenceReward* sequenceReward,
                                         unsigned int idx) {
    auto&& key = keyRewardIdxSeqGiven(sequenceReward->getId());
    CCKeyValueStorage::getInstance()->setValue(
        key, StringUtils::format("%d", idx).c_str());
}

void CCRewardStorage::setTimesGiven(CCReward* reward, bool up, bool notify) {
    int total = getTimesGiven(reward) + (up ? 1 : -1);
    if (total < 0)
        total = 0;

    auto&& key = keyRewardTimesGiven(reward->getId());
    CCKeyValueStorage::getInstance()->setValue(
        key, StringUtils::format("%d", total).c_str());

    if (up) {
        key = keyRewardLastGiven(reward->getId().c_str());

        time_t currentTime;
        time(&currentTime);
        auto val = StringUtils::format(
            "%lld", (long long)(((double)currentTime) * 1000));

        CCKeyValueStorage::getInstance()->setValue(key, val);
    }

    if (notify) {
        if (up) {
            CCCoreEventDispatcher::getInstance()->onRewardGivenEvent(reward);
        } else {
            CCCoreEventDispatcher::getInstance()->onRewardTakenEvent(reward);
        }
    }
}

std::string&& CCRewardStorage::keyRewards(const std::string& rewardId,
                                          const std::string& postfix) {
    return std::move(StringUtils::format("%srewards.%s.%s", DB_KEY_PRFIX,
                                         rewardId.c_str(), postfix.c_str()));
}

std::string&&
CCRewardStorage::keyRewardIdxSeqGiven(const std::string& rewardId) {
    return std::move(keyRewards(rewardId, "seq.id"));
}

std::string&&
CCRewardStorage::keyRewardTimesGiven(const std::string& rewardId) {
    return std::move(keyRewards(rewardId, "timesGiven"));
}

std::string&& CCRewardStorage::keyRewardLastGiven(const std::string& rewardId) {
    return std::move(keyRewards(rewardId, "lastGiven"));
}
} // namespace soomla
