//
//  TimeStrategy.m
//  SoomlaiOSCore
//
//  Created by Refael Dakar on 06/08/14.
//  Copyright (c) 2014 SOOMLA. All rights reserved.
//

#import "Schedule.h"
#import "JSONConsts.h"
#import "SoomlaUtils.h"

@implementation DateTimeRange

@synthesize start, end;

- (id) initWithStartDate:(NSDate*)oStart andEndDate:(NSDate*)oEnd {
    if (self = [super init]) {
        start = oStart;
        end = oEnd;
    }
    
    return self;
}

@end


@implementation Schedule

@synthesize recurrence, timeRanges, activationLimit;

static NSString* TAG = @"SOOMLA Schedule";

+ (Schedule*)Once {
    return [[Schedule alloc] initWithActivationLimit:1];
}

+ (Schedule*)AnyTimeLimited:(int)oActivationLimit {
    return [[Schedule alloc] initWithActivationLimit:oActivationLimit];
}

+ (Schedule*)AnyTimeUnlimited {
    return [[Schedule alloc] initWithActivationLimit:0];
}

- (id)initWithActivationLimit:(int)oActivationLimit {
    return [self initWithTimeRanges:NULL andRecurrence:NONE andActivationLimit:oActivationLimit];
}

- (id)initWithStartDate:(NSDate*)oStart andEndDate:(NSDate*)oEnd andRecurrence:(Recurrence)oRecurrence andActivationLimit:(int)oActivationLimit {
    NSArray* tmpTimeRanges = @[
                            
                            [[DateTimeRange alloc] initWithStartDate:oStart andEndDate:oEnd]
                            
                            ];
    return [self initWithTimeRanges:tmpTimeRanges andRecurrence:oRecurrence andActivationLimit:oActivationLimit];
}

- (id)initWithTimeRanges:(NSArray*)oTimeRanges andRecurrence:(Recurrence)oRecurrence andActivationLimit:(int)oActivationLimit {
    if (self = [super init]) {
        timeRanges = oTimeRanges;
        recurrence = oRecurrence;
        activationLimit = oActivationLimit;
    }
    return self;
}

- (id)initWithDictionary:(NSDictionary *)dict {
    if (self = [super init]) {
        if (dict[SOOM_SCHE_REC]) {
            recurrence = [dict[SOOM_SCHE_REC] intValue];
        } else {
            recurrence = NONE;
        }
        activationLimit = [dict[SOOM_SCHE_APPROVALS] intValue];
        
        NSMutableArray* tmpTimeRanges = [NSMutableArray array];
        if (dict[SOOM_SCHE_RANGES]) {
            NSArray* rangesObjs = [dict[SOOM_SCHE_RANGES] array];
            for(NSDictionary* trDict in rangesObjs) {
                long long tmpTimeMillis = [trDict[SOOM_SCHE_RANGE_START] longLongValue];
                NSDate* start = [NSDate dateWithTimeIntervalSince1970:(tmpTimeMillis/1000.0f)];
                tmpTimeMillis = [trDict[SOOM_SCHE_RANGE_END] longLongValue];
                NSDate* end = [NSDate dateWithTimeIntervalSince1970:(tmpTimeMillis/1000.0f)];
                
                [tmpTimeRanges addObject:[[DateTimeRange alloc] initWithStartDate:start andEndDate:end]];
            }
        }
        timeRanges = tmpTimeRanges;
    }
    
    return self;

}

- (NSDictionary *)toDictionary {
    NSMutableArray* tmpTimeRanges = [NSMutableArray array];
    if (timeRanges) {
        for(DateTimeRange* dtr in timeRanges) {
            long long startMillis = (long long)([dtr.start timeIntervalSince1970] * 1000);
            long long endMillis = (long long)([dtr.end timeIntervalSince1970] * 1000);
            [tmpTimeRanges addObject:@{
                                       SOOM_CLASSNAME: [SoomlaUtils getClassName:dtr],
                                       SOOM_SCHE_RANGE_START: [NSNumber numberWithLongLong:startMillis],
                                       SOOM_SCHE_RANGE_END: [NSNumber numberWithLongLong:endMillis]
                                       }];
        }
    }
    
    return @{
             SOOM_CLASSNAME : [SoomlaUtils getClassName:self],
             SOOM_SCHE_REC: [NSNumber numberWithInt:recurrence],
             SOOM_SCHE_RANGES: tmpTimeRanges,
             SOOM_SCHE_APPROVALS: [NSNumber numberWithInt:activationLimit]
             };
}

- (BOOL)approveWithActivationTimes:(int)activationTimes {
    NSDate* now = [NSDate date];
    
    if (activationLimit < 1 && (!timeRanges || [timeRanges count] == 0)) {
        LogDebug(TAG, @"There's no activation limit and no TimeRanges. APPROVED!");
        return YES;
    }
    
    if (activationLimit>0 && activationTimes >= activationLimit) {
        LogDebug(TAG, @"Activation limit exceeded.");
        return NO;
    }
    
    if ((!timeRanges || [timeRanges count] == 0)) {
        LogDebug(TAG, @"We have an activation limit that was not reached. Also, we don't have any time ranges. APPROVED!");
        return YES;
    }
    
    // NOTE: From this point on ... we know that we didn't reach the activation limit AND we have TimeRanges.
    //		 We'll just make sure the time ranges and the Recurrence copmlies.
    
    for(DateTimeRange* dtr in timeRanges) {
        if (([now compare:dtr.start] == NSOrderedDescending) && ([now compare:dtr.end] == NSOrderedAscending)) {
            LogDebug(TAG, @"We are just in one of the time spans, it can't get any better then that. APPROVED!");
            return YES;
        }
    }
    
    // we don't need to continue if RequiredRecurrence is NONE
    if (recurrence == NONE) {
        return NO;
    }
    
    for(DateTimeRange* dtr in timeRanges) {
        
        NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
        NSDateComponents *nowComponents = [calendar components:(NSCalendarUnitHour | NSCalendarUnitMinute) fromDate:now];
        NSDateComponents *startComponents = [calendar components:(NSCalendarUnitHour | NSCalendarUnitMinute) fromDate:dtr.start];
        NSDateComponents *endComponents = [calendar components:(NSCalendarUnitHour | NSCalendarUnitMinute) fromDate:dtr.end];
        
        if (nowComponents.minute >= startComponents.minute &&
            nowComponents.minute <= endComponents.minute) {
            LogDebug(TAG, @"Now is in one of the time ranges' minutes span.");
            
            if (recurrence == EVERY_HOUR) {
                LogDebug(TAG, @"It's a EVERY_HOUR recurrence. APPROVED!");
                return YES;
            }
            
            if (nowComponents.hour >= startComponents.hour &&
                nowComponents.hour <= endComponents.hour) {
                LogDebug(TAG, @"Now is in one of the time ranges' hours span.");
                
                if (recurrence == EVERY_DAY) {
                    LogDebug(TAG, @"It's a EVERY_DAY recurrence. APPROVED!");
                    return YES;
                }
                
                if (nowComponents.weekday >= startComponents.weekday
                    && nowComponents.weekday <= endComponents.weekday) {
                    LogDebug(TAG, @"Now is in one of the time ranges' day-of-week span.");
                    
                    if (recurrence == EVERY_WEEK) {
                        LogDebug(TAG, @"It's a EVERY_WEEK recurrence. APPROVED!");
                        return YES;
                    }
                    
                    if (nowComponents.day >= startComponents.day
                        && nowComponents.day <= endComponents.day) {
                        LogDebug(TAG, @"Now is in one of the time ranges' days span.");
                        
                        if (recurrence == EVERY_MONTH) {
                            LogDebug(TAG, @"It's a EVERY_MONTH recurrence. APPROVED!");
                            return YES;
                        }
                    }
                }
            }
        }
        
    }
    return NO;
}

@end
