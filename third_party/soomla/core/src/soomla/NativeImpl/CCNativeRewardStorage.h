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

#ifndef __CCNativeRewardStorage_H__
#define __CCNativeRewardStorage_H__

#include "soomla/data/CCRewardStorage.h"

namespace soomla {
class CCNativeRewardStorage : public CCRewardStorage {
public:
    virtual void setRewardStatus(CCReward* reward, bool give, bool notify);
    virtual int getTimesGiven(CCReward* reward);
    virtual int getLastSeqIdxGiven(CCSequenceReward* sequenceReward);
    virtual void setLastSeqIdxGiven(CCSequenceReward* sequenceReward,
                                    unsigned int idx);
};
} // namespace soomla

#endif // __CCNativeRewardStorage_H__
