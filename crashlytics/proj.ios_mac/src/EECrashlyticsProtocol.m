//
//  EECrashlyticsProtocol.m
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#import <UIKit/UIKit.h>
#import <Crashlytics/Crashlytics.h>

#import "EECrashlyticsProtocol.h"

#undef CLS_LOG
#ifdef __OBJC__
#ifndef NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSNSLog(__FORMAT__, ##__VA_ARGS__)
#else
#define CLS_LOG(__FORMAT__, ...) CLSLog(__FORMAT__, ##__VA_ARGS__)
#endif
#endif

// clang-format off

@implementation EECrashlyticsProtocol

- (id) init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    return self;
}

- (void) causeCrash {
    [[Crashlytics sharedInstance] crash];
}

- (void) causeException {
    [[Crashlytics sharedInstance] throwException];
}

- (void) log:(NSDictionary* _Nonnull) dict {
    NSAssert([dict count] == 3, @"...");
    
    NSString* priorityDescription = dict[@"priorityDescription"];
    NSString* tag = dict[@"tag"];
    NSString* message = dict[@"message"];
    
    NSAssert(priorityDescription != nil, @"...");
    NSAssert(tag != nil, @"...");
    NSAssert(message != nil, @"...");
    
    CLS_LOG(@"%s %s: %s",
            [priorityDescription UTF8String],
            [tag UTF8String],
            [message UTF8String]);
}

- (void) setString:(NSDictionary* _Nonnull) dict {
    NSAssert([dict count] == 2, @"...");
    
    NSString* key = dict[@"key"];
    NSString* value = dict[@"value"];
    
    NSAssert(key != nil, @"...");
    NSAssert(value != nil, @"...");
    
    [[Crashlytics sharedInstance] setObjectValue:value forKey:key];
}

- (void) setBool:(NSDictionary* _Nonnull) dict {
    NSAssert([dict count] == 2, @"...");
    
    NSString* key = dict[@"key"];
    NSNumber* value = dict[@"value"];
    
    NSAssert(key != nil, @"...");
    NSAssert(value != nil, @"...");
    
    [[Crashlytics sharedInstance] setBoolValue:[value boolValue] forKey:key];
}

- (void) setInt:(NSDictionary* _Nonnull) dict {
    NSAssert([dict count] == 2, @"...");
    
    NSString* key = dict[@"key"];
    NSNumber* value = dict[@"value"];
    
    NSAssert(key != nil, @"...");
    NSAssert(value != nil, @"...");
    
    [[Crashlytics sharedInstance] setIntValue:[value intValue] forKey:key];
}

- (void) setUserIdentifier:(NSDictionary* _Nonnull) dict {
    NSAssert([dict count] == 1, @"...");
    
    NSString* identifier = dict[@"identifier"];
    
    NSAssert(identifier != nil, @"...");
    [[Crashlytics sharedInstance] setUserIdentifier:identifier];
}

- (void) setUserName:(NSDictionary* _Nonnull) dict {
    NSAssert([dict count] == 1, @"...");
    
    NSString* name = dict[@"name"];
    
    NSAssert(name != nil, @"...");
    
    [[Crashlytics sharedInstance] setUserName:name];
}

- (void) setUserEmail:(NSDictionary* _Nonnull) dict {
    NSAssert([dict count] == 1, @"...");
    
    NSString* email = dict[@"email"];
    
    NSAssert(email != nil, @"...");
    
    [[Crashlytics sharedInstance] setUserEmail:email];
}

@end

// clang-format on