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

#include "CCError.h"
#include "CCNativeRewardStorage.h"
#include "CCNdkBridge.h"
#include "CCSoomlaUtils.h"

namespace soomla {

#define TAG "SOOMLA NativeRewardStorage"

USING_NS_CC;

void CCNativeRewardStorage::setRewardStatus(CCReward* reward, bool give,
                                            bool notify) {
    CCError* error = NULL;
    
    ValueMap params;
    params["method"] = "CCNativeRewardStorage::setRewardStatus";
    params["reward"] = reward->getId();
    params["give"] =  give;
    params["notify"] = notify;
    CCNdkBridge::callNative(params, &error);

    if (error) {
        CCSoomlaUtils::logException(TAG, error);
        CC_ASSERT(false);
    }
}

int CCNativeRewardStorage::getTimesGiven(CCReward* reward) {
    CCError* error = NULL;
    
    ValueMap params;
    params["method"] = "CCNativeRewardStorage::getTimesGiven";
    params["reward"] = reward->getId();
    
    ValueMap retParams =
        CCNdkBridge::callNative(params, &error);

    if (error) {
        CCSoomlaUtils::logException(TAG, error);
        CC_ASSERT(false);
        return -1;
    }

    if (!retParams.count("return")) {
        return -1;
    }
    
    return retParams.at("return").asInt();
}

int CCNativeRewardStorage::getLastSeqIdxGiven(
    CCSequenceReward* sequenceReward) {
    CCError* error = NULL;

    ValueMap params;
    params["method"] = "CCNativeRewardStorage::getLastSeqIdxGiven";
    params["reward"] = sequenceReward->getId();
    
    ValueMap  retParams = CCNdkBridge::callNative(params, &error);

    if (error) {
        CCSoomlaUtils::logException(TAG, error);
        CC_ASSERT(false);
        return -1;
    }
    
    if (not retParams.count("return")) {
        return -1;
    }

    return retParams.at("return").asInt();
}

void CCNativeRewardStorage::setLastSeqIdxGiven(CCSequenceReward* sequenceReward,
                                               unsigned int idx) {
    CCError* error = NULL;

    ValueMap params;
    params["method"] = "CCNativeRewardStorage::setLastSeqIdxGiven";
    params["reward"] = sequenceReward->getId();
    params["idx"] = idx;
    CCNdkBridge::callNative(params, &error);

    if (error) {
        CCSoomlaUtils::logException(TAG, error);
        CC_ASSERT(false);
    }
}
} // namespace soomla
