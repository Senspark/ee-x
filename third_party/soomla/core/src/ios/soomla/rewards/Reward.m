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

#import "Reward.h"
#import "JSONConsts.h"
#import "RewardStorage.h"
#import "SoomlaUtils.h"
#import "DictionaryFactory.h"
#import "Schedule.h"

@implementation Reward

@synthesize schedule;

static NSString* TAG = @"SOOMLA Reward";
static DictionaryFactory* dictionaryFactory;


- (id)initWithRewardId:(NSString *)oRewardId andName:(NSString *)oName {
    self = [super initWithName:oName andDescription:@"" andID:oRewardId];
    if ([self class] == [Reward class]) {
        @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                       reason:@"Error, attempting to instantiate AbstractClass directly." userInfo:nil];
    }
    
    if (self) {
        self.schedule = [Schedule Once];
    }
    return self;
}

- (id)initWithDictionary:(NSDictionary *)dict {
    self = [super initWithDictionary:dict];
    if ([self class] == [Reward class]) {
        @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                       reason:@"Error, attempting to instantiate AbstractClass directly." userInfo:nil];
    }
    
    if (self) {
        if (dict[SOOM_SCHEDULE]) {
            self.schedule = [[Schedule alloc] initWithDictionary:dict[SOOM_SCHEDULE]];
        } else {
            self.schedule = [Schedule Once];
        }
    }
    
    return self;
}

- (NSDictionary *)toDictionary {
    NSDictionary* parentDict = [super toDictionary];
    
    NSMutableDictionary* toReturn = [[NSMutableDictionary alloc] initWithDictionary:parentDict];
    if (self.schedule) {
        [toReturn setObject:[schedule toDictionary] forKey:SOOM_SCHEDULE];
    } else {
        [toReturn setObject:[[Schedule Once] toDictionary] forKey:SOOM_SCHEDULE];
    }
    
    return toReturn;
}

- (BOOL)give {
    if (![self canGive]) {
        LogDebug(TAG, ([NSString stringWithFormat:@"(Give) Reward is not approved by Schedule. id: %@", self.ID]));
        return NO;
    }

    if ([self giveInner]) {
        [RewardStorage setStatus:YES forReward:self.ID];
        return YES;
    }
    
    return NO;
}

- (BOOL)take {
    if ([RewardStorage isRewardGiven:self.ID]) {
        LogDebug(TAG, ([NSString stringWithFormat:@"Reward not give. id: %@", self.ID]));
        return NO;
    }
    
    if ([self takeInner]) {
        [RewardStorage setStatus:NO forReward:self.ID];
        return YES;
    }
    return NO;
}

- (BOOL)canGive {
    return [schedule approveWithActivationTimes:[RewardStorage getTimesGivenForReward:self.ID]];
}

- (BOOL)isOwned {
    return [RewardStorage isRewardGiven:self.ID];
}


// Abstract methods

- (BOOL)giveInner {
    @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                   reason:[NSString stringWithFormat:@"You must override %@ in a subclass",
                                           NSStringFromSelector(_cmd)]
                                 userInfo:nil];
}

- (BOOL)takeInner {
    @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                   reason:[NSString stringWithFormat:@"You must override %@ in a subclass",
                                           NSStringFromSelector(_cmd)]
                                 userInfo:nil];
}


// Static methods

+ (Reward *)fromDictionary:(NSDictionary *)dict {
    return (Reward *)[dictionaryFactory createObjectWithDictionary:dict];
}

+ (void)initialize {
    if (self == [Reward self]) {
        dictionaryFactory = [[DictionaryFactory alloc] init];
    }
}


@end
