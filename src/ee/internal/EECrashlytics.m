//
//  EECrashlyticsProtocol.m
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#import <Crashlytics/Crashlytics.h>

#import "ee/internal/EECrashlytics.h"
#import "ee/internal/EEDictionaryUtils.h"
#import "ee/internal/EEJsonUtils.h"
#import "ee/internal/EEMessageBridge.h"

#undef CLS_LOG
#ifdef __OBJC__
#ifndef NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSNSLog(__FORMAT__, ##__VA_ARGS__)
#else // NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSLog(__FORMAT__, ##__VA_ARGS__)
#endif // NDEBUG
#endif // __OBJC__

@implementation EECrashlytics

NSString* const k__crashlytics_cause_crash = @"__crashlytics_cause_crash";
NSString* const k__crashlytics_cause_exception =
    @"__crashlytics_cause_exception";
NSString* const k__crashlytics_set_log_level = @"k__crashlytics_set_log_level";
NSString* const k__crashlytics_log = @"__crashlytics_log";
NSString* const k__crashlytics_set_string = @"__crashlytics_set_string";
NSString* const k__crashlytics_set_bool = @"__crashlytics_set_bool";
NSString* const k__crashlytics_set_int = @"__crashlytics_set_int";
NSString* const k__crashlytics_set_user_identifier =
    @"__crashlytics_set_user_identifier";
NSString* const k__crashlytics_set_user_name = @"__crashlytics_set_user_name";
NSString* const k__crashlytics_set_user_email = @"__crashlytics_set_user_email";

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:^(NSString* msg) {
        [self causeCrash];
        return [EEDictionaryUtils emptyResult];
    } tag:k__crashlytics_cause_crash];

    [bridge registerHandler:^(NSString* msg) {
        [self causeException];
        return [EEDictionaryUtils emptyResult];
    } tag:k__crashlytics_cause_exception];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 1, @"...");

        // Ignored.

        return [EEDictionaryUtils emptyResult];
    } tag:k__crashlytics_set_log_level];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 4, @"...");

        NSString* priorityDescription = dict[@"priorityDescription"];
        NSString* tag = dict[@"tag"];
        NSString* message = dict[@"message"];

        NSAssert(priorityDescription != nil, @"...");
        NSAssert(tag != nil, @"...");
        NSAssert(message != nil, @"...");

        [self log:priorityDescription tag:tag message:message];
        return [EEDictionaryUtils emptyResult];
    } tag:k__crashlytics_log];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 2, @"...");

        NSString* key = dict[@"key"];
        NSString* value = dict[@"value"];

        NSAssert(key != nil, @"...");
        NSAssert(value != nil, @"...");

        [self setString:key value:value];
        return [EEDictionaryUtils emptyResult];
    } tag:k__crashlytics_set_string];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 2, @"...");

        NSString* key = dict[@"key"];
        NSNumber* value = dict[@"value"];

        NSAssert(key != nil, @"...");
        NSAssert(value != nil, @"...");

        [self setBool:key value:value];
        return [EEDictionaryUtils emptyResult];
    } tag:k__crashlytics_set_bool];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 2, @"...");

        NSString* key = dict[@"key"];
        NSNumber* value = dict[@"value"];

        NSAssert(key != nil, @"...");
        NSAssert(value != nil, @"...");

        [self setInt:key value:value];
        return [EEDictionaryUtils emptyResult];
    } tag:k__crashlytics_set_int];

    [bridge registerHandler:^(NSString* msg) {
        [self setUserIdentifier:msg];
        return [EEDictionaryUtils emptyResult];
    } tag:k__crashlytics_set_user_identifier];

    [bridge registerHandler:^(NSString* msg) {
        [self setUserName:msg];
        return [EEDictionaryUtils emptyResult];
    } tag:k__crashlytics_set_user_name];

    [bridge registerHandler:^(NSString* msg) {
        [self setUserEmail:msg];
        return [EEDictionaryUtils emptyResult];
    } tag:k__crashlytics_set_user_email];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__crashlytics_cause_crash];
    [bridge deregisterHandler:k__crashlytics_cause_exception];
    [bridge deregisterHandler:k__crashlytics_log];
    [bridge deregisterHandler:k__crashlytics_set_log_level];
    [bridge deregisterHandler:k__crashlytics_set_string];
    [bridge deregisterHandler:k__crashlytics_set_bool];
    [bridge deregisterHandler:k__crashlytics_set_int];
    [bridge deregisterHandler:k__crashlytics_set_user_identifier];
    [bridge deregisterHandler:k__crashlytics_set_user_name];
    [bridge deregisterHandler:k__crashlytics_set_user_email];
}

- (void)causeCrash {
    [[Crashlytics sharedInstance] crash];
}

- (void)causeException {
    [[Crashlytics sharedInstance] throwException];
}

- (void)log:(NSString* _Nonnull)desc
        tag:(NSString* _Nonnull)tag
    message:(NSString* _Nonnull)message {
    CLS_LOG(@"%s %s: %s", [desc UTF8String], [tag UTF8String],
            [message UTF8String]);
}

- (void)setString:(NSString* _Nonnull)key value:(NSString* _Nonnull)value {
    [[Crashlytics sharedInstance] setObjectValue:value forKey:key];
}

- (void)setBool:(NSString* _Nonnull)key value:(NSNumber* _Nonnull)value {
    [[Crashlytics sharedInstance] setBoolValue:[value boolValue] forKey:key];
}

- (void)setInt:(NSString* _Nonnull)key value:(NSNumber* _Nonnull)value {
    [[Crashlytics sharedInstance] setIntValue:[value intValue] forKey:key];
}

- (void)setUserIdentifier:(NSString* _Nonnull)identifier {
    [[Crashlytics sharedInstance] setUserIdentifier:identifier];
}

- (void)setUserName:(NSString* _Nonnull)name {
    [[Crashlytics sharedInstance] setUserName:name];
}

- (void)setUserEmail:(NSString* _Nonnull)email {
    [[Crashlytics sharedInstance] setUserEmail:email];
}

@end
