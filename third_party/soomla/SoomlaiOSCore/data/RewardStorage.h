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

@class Reward;
@class SequenceReward;

@interface RewardStorage : NSObject

+ (void)setStatus:(BOOL)status forReward:(NSString *)rewardId;

+ (void)setStatus:(BOOL)status forReward:(NSString *)rewardId andNotify:(BOOL)notify;

+ (BOOL)isRewardGiven:(NSString *)rewardId;

+ (int)getLastSeqIdxGivenForSequenceReward:(NSString *)sequenceReward;

+ (void)setLastSeqIdxGiven:(int)idx ForSequenceReward:(NSString *)sequenceReward;

+ (int)getTimesGivenForReward:(NSString*)rewardId;

+ (void)setTimesGivenForReward:(NSString*)rewardId up:(BOOL)up andNotify:(BOOL)notify;

+ (void)resetTimesGivenForReward:(NSString*)rewardId andTimesGiven:(int)timesGiven;

+ (NSDate*)getLastGivenTimeForReward:(NSString*)rewardId;

+ (long long)getLastGivenTimeMillisForReward:(NSString*)rewardId;

+ (void)setLastGivenTimeMillisForReward:(NSString*)rewardId andLastGivenTimeMillis:(long long)lastGiven;

+ (NSDictionary *)getRewardsState;

+ (BOOL)resetRewardsState:(NSDictionary *)state;

@end
