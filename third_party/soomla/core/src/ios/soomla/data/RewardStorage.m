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

#import "RewardStorage.h"
#import "Reward.h"
#import "SequenceReward.h"
#import "SoomlaEventHandling.h"
#import "KeyValueStorage.h"
#import "SoomlaConfig.h"
#import "SoomlaUtils.h"

@implementation RewardStorage


+ (void)setStatus:(BOOL)status forReward:(NSString *)rewardId {
    [self setStatus:status forReward:rewardId andNotify:YES];
}

+ (void)setStatus:(BOOL)status forReward:(NSString *)rewardId andNotify:(BOOL)notify {
    [self setTimesGivenForReward:rewardId up:status andNotify:notify];
}

+ (BOOL)isRewardGiven:(NSString *)rewardId {
    return [self getTimesGivenForReward:rewardId] > 0;
}

+ (int)getLastSeqIdxGivenForSequenceReward:(NSString *)sequenceRewardId {
    NSString* key = [self keyRewardIdxSeqGivenWithRewardId:sequenceRewardId];
    NSString* val = [KeyValueStorage getValueForKey:key];
    
    if (!val || [val length] == 0){
        return -1;
    }
    
    return [val intValue];
}

+ (void)setLastSeqIdxGiven:(int)idx ForSequenceReward:(NSString *)sequenceRewardId {
    NSString* key = [self keyRewardIdxSeqGivenWithRewardId:sequenceRewardId];
    NSString* val = [[NSNumber numberWithInt:idx] stringValue];
    
    [KeyValueStorage setValue:val forKey:key];
}

+ (void)setTimesGivenForReward:(NSString*)rewardId up:(BOOL)up andNotify:(BOOL)notify {
    int total = [self getTimesGivenForReward:rewardId] + (up ? 1 : -1);
    
    [self resetTimesGivenForReward:rewardId andTimesGiven:total];
    
    if (up) {
        [self setLastGivenTimeMillisForReward:rewardId
                       andLastGivenTimeMillis:(long long)([[NSDate date] timeIntervalSince1970] * 1000)];
    }
    
    if (notify) {
        if (up) {
            [SoomlaEventHandling postRewardGiven:rewardId];
        } else {
            [SoomlaEventHandling postRewardTaken:rewardId];
        }
    }
}

+ (void)resetTimesGivenForReward:(NSString*)rewardId andTimesGiven:(int)timesGiven {
    NSString* key = [self keyRewardTimesGiven:rewardId];
    NSString* val = [[NSNumber numberWithInt:timesGiven] stringValue];
    
    [KeyValueStorage setValue:val forKey:key];
}

+ (void)setLastGivenTimeMillisForReward:(NSString*)rewardId andLastGivenTimeMillis:(long long)lastGiven {
    NSString *key = [self keyRewardLastGiven:rewardId];
    NSString *val = [NSString stringWithFormat:@"%lld",lastGiven];
    
    [KeyValueStorage setValue:val forKey:key];
}

+ (int)getTimesGivenForReward:(NSString*)rewardId {
    NSString* key = [self keyRewardTimesGiven:rewardId];
    NSString* val = [KeyValueStorage getValueForKey:key];
    if (!val || [val length] == 0){
        return 0;
    }
    return [val intValue];
}

+ (NSDate*)getLastGivenTimeForReward:(NSString*)rewardId {
    long long timeMillis = [self getLastGivenTimeMillisForReward:rewardId];
    if (timeMillis == 0) {
        return NULL;
    }
    return [NSDate dateWithTimeIntervalSince1970:(timeMillis/1000)];
}

+ (long long)getLastGivenTimeMillisForReward:(NSString*)rewardId {
    NSString* key = [self keyRewardLastGiven:rewardId];
    NSString* val = [KeyValueStorage getValueForKey:key];
    if (!val || [val length] == 0){
        return 0;
    }
    return [val longLongValue];
}

+ (NSDictionary *)getRewardsState {
    NSArray *rewardIds = [self getRewardIds];
    NSMutableDictionary *rewardStateDict = [NSMutableDictionary dictionary];
    
    for (NSString *rewardId in rewardIds) {
        NSMutableDictionary *rewardValuesDict = [NSMutableDictionary dictionary];
        @try {
            int timesGiven = [self getTimesGivenForReward:rewardId];
            rewardValuesDict[@"timesGiven"] = [NSNumber numberWithInt:timesGiven];
            
            long long lastGiven = [self getLastGivenTimeMillisForReward:rewardId];
            rewardValuesDict[@"lastGiven"] = [NSNumber numberWithLongLong:lastGiven];
            
            rewardStateDict[rewardId] = rewardValuesDict;
        }
        @catch (NSException *exception) {
            LogDebug(TAG, ([NSString stringWithFormat:@"Unable to set reward %@ state. error: %@", rewardId, exception.description]));
        }
    }
    
    return rewardStateDict;
}

+ (BOOL)resetRewardsState:(NSDictionary *)state {
    if (!state) {
        return NO;
    }
    
    NSMutableArray *rewardIds = [self getRewardIds];
    @try {
        for (NSString *rewardId in state) {
            NSDictionary *rewardValuesDict = state[rewardId];
            
            NSNumber *timesGiven = rewardValuesDict[@"timesGiven"];
            if (timesGiven) {
                [self resetTimesGivenForReward:rewardId andTimesGiven:[timesGiven intValue]];
            }
            
            NSNumber *lastGiven = rewardValuesDict[@"lastGiven"];
            if (lastGiven) {
                [self setLastGivenTimeMillisForReward:rewardId andLastGivenTimeMillis:[lastGiven longLongValue]];
            }
            
            [rewardIds removeObject:rewardId];
        }
    }
    @catch (NSException *exception) {
        LogError(TAG, ([NSString stringWithFormat:@"Unable to set state for rewards. error: %@", exception.description]));
        return NO;
    }
    
    // When resetting state we should remove all rewards' state which
    // were not in the sync state (so the state is inline with the provided
    // state)
    for (NSString *rewardId in rewardIds) {
        [KeyValueStorage deleteValueForKey:[self keyRewardTimesGiven:rewardId]];
        [KeyValueStorage deleteValueForKey:[self keyRewardLastGiven:rewardId]];
        [KeyValueStorage deleteValueForKey:[self keyRewardIdxSeqGivenWithRewardId:rewardId]];
        
    }
    
    return YES;
}

// Private

+ (NSMutableArray *)getRewardIds {
    NSArray *kvKeys = [KeyValueStorage getEncryptedKeys];
    NSMutableArray *rewardIds = [NSMutableArray array];
    
    if (!kvKeys || (kvKeys.count == 0)) {
        return rewardIds;
    }
    
    NSString *rewardsPrefix = [NSString stringWithFormat: @"%@rewards.", DB_KEY_PREFIX];
    for (NSString *key in kvKeys) {
        NSRange findRange = [key rangeOfString:rewardsPrefix];
        if (findRange.length > 0) {
            NSString *rewardId = [key stringByReplacingOccurrencesOfString:rewardsPrefix withString:@""];
            NSRange dotRange = [rewardId rangeOfString:@"."];
            if (dotRange.length > 0) {
                rewardId = [rewardId substringToIndex:dotRange.location];
            }
            if ([rewardIds indexOfObject:rewardId] == NSNotFound) {
                [rewardIds addObject:rewardId];
            }
        }
    }
    
    return rewardIds;
}

static NSString* TAG = @"SOOMLA RewardStorage";

+ (NSString *)keyRewardsWithRewardId:(NSString *)rewardId AndPostfix:(NSString *)postfix {
    return [NSString stringWithFormat: @"%@rewards.%@.%@", DB_KEY_PREFIX, rewardId, postfix];
}

+ (NSString *)keyRewardTimesGiven:(NSString *)rewardId {
    return [self keyRewardsWithRewardId:rewardId AndPostfix:@"timesGiven"];
}

+ (NSString *)keyRewardLastGiven:(NSString *)rewardId {
    return [self keyRewardsWithRewardId:rewardId AndPostfix:@"lastGiven"];
}

+ (NSString *)keyRewardIdxSeqGivenWithRewardId:(NSString *)rewardId {
    return [self keyRewardsWithRewardId:rewardId AndPostfix:@"seq.idx"];
}

@end
