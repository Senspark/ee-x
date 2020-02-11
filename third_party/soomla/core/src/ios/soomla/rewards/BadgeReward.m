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

#import "BadgeReward.h"
#import "JSONConsts.h"

@implementation BadgeReward

@synthesize iconUrl;

- (id)initWithRewardId:(NSString *)oRewardId andName:(NSString *)oName andIconUrl:(NSString *)oIconUrl {
    if (self = [super initWithRewardId:oRewardId andName:oName]) {
        self.iconUrl = oIconUrl;
    }
    
    return self;
}

- (id)initWithDictionary:(NSDictionary *)dict {
    if (self = [super initWithDictionary:dict]) {
        self.iconUrl = dict[SOOM_REWARD_ICONURL];
    }
    
    return self;
}

- (NSDictionary *)toDictionary {
    NSDictionary* parentDict = [super toDictionary];
    
    NSMutableDictionary* toReturn = [[NSMutableDictionary alloc] initWithDictionary:parentDict];
    [toReturn setObject:(self.iconUrl ?: @"") forKey:SOOM_REWARD_ICONURL];
    
    return toReturn;
}

- (BOOL)giveInner {
    
    // nothing to do here... the parent Reward gives in storage
    return YES;
}

- (BOOL)takeInner {
    
    // nothing to do here... the parent Reward takes in storage
    return YES;
}

@end
