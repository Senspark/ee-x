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

#import "SequenceReward.h"
#import "JSONConsts.h"
#import "BadgeReward.h"
#import "RewardStorage.h"
#import "SoomlaUtils.h"

@implementation SequenceReward

@synthesize rewards;

static NSString* TAG = @"SOOMLA SequenceReward";

- (id)initWithRewardId:(NSString *)oRewardId andName:(NSString *)oName andRewards:(NSArray *)oRewards {
    if (self = [super initWithRewardId:oRewardId andName:oName]) {
        
        if (![oRewards count]) {
            LogError(TAG, @"this reward doesn't make sense without items");
        }

        self.rewards = oRewards;
    }
    
    return self;
}


- (id)initWithDictionary:(NSDictionary *)dict {
    if (self = [super initWithDictionary:dict]) {
        
        NSMutableArray* tmpRewards = [NSMutableArray array];
        NSArray* rewardsArr = dict[SOOM_REWARDS];
        
        if (!rewardsArr) {
            LogDebug(TAG, @"reward has no meaning without children");
            rewardsArr = [NSMutableArray array];
        }
        
        // Iterate over all rewards in the JSON array and for each one create
        // an instance according to the reward type
        for (NSDictionary* rewardDict in rewardsArr) {
            
            Reward* reward = [Reward fromDictionary:rewardDict];
            if (reward) {
                [tmpRewards addObject:reward];
            }
        }
        
        self.rewards = tmpRewards;
    }
    
    return self;
}

- (NSDictionary *)toDictionary {
    NSDictionary* parentDict = [super toDictionary];
    
    NSMutableArray* rewardsArr = [NSMutableArray array];
    for (Reward* reward in self.rewards) {
        [rewardsArr addObject:[reward toDictionary]];
    }
    
    NSMutableDictionary* toReturn = [[NSMutableDictionary alloc] initWithDictionary:parentDict];
    [toReturn setObject:rewardsArr forKey:SOOM_REWARDS];
    
    return toReturn;
}




- (Reward *)getLastGivenReward {
    int idx = [RewardStorage getLastSeqIdxGivenForSequenceReward:self.ID];
    if (idx < 0) {
        return nil;
    }
    return self.rewards[idx];
}

- (BOOL)hasMoreToGive {
    return [RewardStorage getLastSeqIdxGivenForSequenceReward:self.ID] < [self.rewards count] ;
}

- (BOOL)forceGiveNextReward:(Reward *)reward {
    for (int i = 0; i < [self.rewards count]; i++) {
        if ([((Reward*)self.rewards[i]).ID isEqualToString:reward.ID]) {
            [RewardStorage setLastSeqIdxGiven:(i - 1) ForSequenceReward:self.ID];
            return YES;
        }
    }

    return NO;
}

- (BOOL)giveInner {
    int idx = [RewardStorage getLastSeqIdxGivenForSequenceReward:self.ID];
    if (idx >= [rewards count]) {
        return NO; // all rewards in the sequence were given
    }
    
    [RewardStorage setLastSeqIdxGiven:(++idx) ForSequenceReward:self.ID];
    return YES;
}

- (BOOL)takeInner {
    int idx = [RewardStorage getLastSeqIdxGivenForSequenceReward:self.ID];
    if (idx <= 0) {
        return NO; // all rewards in the sequence were taken
    }
    
    [RewardStorage setLastSeqIdxGiven:(--idx) ForSequenceReward:self.ID];
    return YES;
}


@end
