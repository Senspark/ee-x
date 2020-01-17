//
//  EECrashlyticsProtocol.m
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/crashlytics/EECrashlytics.h"

#import <Crashlytics/Crashlytics.h>
#import <Fabric/Fabric.h>

#import <ee/core/internal/EEDictionaryUtils.h>
#import <ee/core/internal/EEJsonUtils.h>
#import <ee/core/internal/EEMessageBridge.h>

#undef CLS_LOG
#ifdef __OBJC__
#ifndef NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSNSLog(__FORMAT__, ##__VA_ARGS__)
#else // NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSLog(__FORMAT__, ##__VA_ARGS__)
#endif // NDEBUG
#endif // __OBJC__

@implementation EECrashlytics {
    EEMessageBridge* bridge_;
}

// clang-format off
NSString* const k__crashlytics_cause_crash          = @"__crashlytics_cause_crash";
NSString* const k__crashlytics_cause_exception      = @"__crashlytics_cause_exception";
NSString* const k__crashlytics_set_log_level        = @"__crashlytics_set_log_level";
NSString* const k__crashlytics_log                  = @"__crashlytics_log";
NSString* const k__crashlytics_set_string           = @"__crashlytics_set_string";
NSString* const k__crashlytics_set_bool             = @"__crashlytics_set_bool";
NSString* const k__crashlytics_set_int              = @"__crashlytics_set_int";
NSString* const k__crashlytics_set_user_identifier  = @"__crashlytics_set_user_identifier";
NSString* const k__crashlytics_set_user_name        = @"__crashlytics_set_user_name";
NSString* const k__crashlytics_set_user_email       = @"__crashlytics_set_user_email";
NSString* const k__crashlytics_track_level_start    = @"__crashlytics_track_level_start";
NSString* const k__crashlytics_track_level_end      = @"__crashlytics_track_level_end";
NSString* const k__crashlytics_track_purchase       = @"__crashlytics_track_purchase";
NSString* const k__crashlytics_track_custom_event   = @"__crashlytics_track_custom_event";
NSString* const k__crashlytics_track_invite         = @"__crashlytics_track_invite";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }

    // Initialized automatically.
    // https://firebase.google.com/docs/crashlytics/upgrade-from-crash-reporting#ios
    // [Fabric with:@[[Crashlytics class], [Answers class]]];

    bridge_ = [EEMessageBridge getInstance];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:^(NSString* msg) {
        [self causeCrash];
        return @"";
    }
                         tag:k__crashlytics_cause_crash];

    [bridge_ registerHandler:^(NSString* msg) {
        [self causeException];
        return @"";
    }
                         tag:k__crashlytics_cause_exception];

    [bridge_ registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 1, @"...");

        // Ignored.

        return @"";
    }
                         tag:k__crashlytics_set_log_level];

    [bridge_ registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 4, @"...");

        NSString* priorityDescription = dict[@"priorityDescription"];
        NSString* tag = dict[@"tag"];
        NSString* message = dict[@"message"];

        NSAssert(priorityDescription != nil, @"...");
        NSAssert(tag != nil, @"...");
        NSAssert(message != nil, @"...");

        [self log:priorityDescription tag:tag message:message];
        return @"";
    }
                         tag:k__crashlytics_log];

    [bridge_ registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 2, @"...");

        NSString* key = dict[@"key"];
        NSString* value = dict[@"value"];

        NSAssert(key != nil, @"...");
        NSAssert(value != nil, @"...");

        [self setString:key value:value];
        return @"";
    }
                         tag:k__crashlytics_set_string];

    [bridge_ registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 2, @"...");

        NSString* key = dict[@"key"];
        NSNumber* value = dict[@"value"];

        NSAssert(key != nil, @"...");
        NSAssert(value != nil, @"...");

        [self setBool:key value:value];
        return @"";
    }
                         tag:k__crashlytics_set_bool];

    [bridge_ registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 2, @"...");

        NSString* key = dict[@"key"];
        NSNumber* value = dict[@"value"];

        NSAssert(key != nil, @"...");
        NSAssert(value != nil, @"...");

        [self setInt:key value:value];
        return @"";
    }
                         tag:k__crashlytics_set_int];

    [bridge_ registerHandler:^(NSString* msg) {
        [self setUserIdentifier:msg];
        return @"";
    }
                         tag:k__crashlytics_set_user_identifier];

    [bridge_ registerHandler:^(NSString* msg) {
        [self setUserName:msg];
        return @"";
    }
                         tag:k__crashlytics_set_user_name];

    [bridge_ registerHandler:^(NSString* msg) {
        [self setUserEmail:msg];
        return @"";
    }
                         tag:k__crashlytics_set_user_email];

    [bridge_ registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 2, @"...");

        NSString* name = dict[@"name"];
        NSDictionary* attrs = dict[@"attrs"];

        [self trackLevelStart:name customAttributes:attrs];
        return @"";
    }
                         tag:k__crashlytics_track_level_start];

    [bridge_ registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 4, @"...");

        NSString* name = dict[@"name"];
        NSNumber* score = dict[@"score"];
        NSNumber* success = dict[@"success"];
        NSDictionary* attrs = dict[@"attrs"];

        [self trackLevelEnd:name
                       score:score
                     success:success
            customAttributes:attrs];
        return @"";
    }
                         tag:k__crashlytics_track_level_end];

    [bridge_ registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 7, @"...");

        NSNumber* price = dict[@"price"];
        NSString* currency = dict[@"currency"];
        NSNumber* success = dict[@"success"];
        NSString* itemName = dict[@"item_name"];
        NSString* itemType = dict[@"item_type"];
        NSString* itemId = dict[@"item_id"];
        NSDictionary* attrs = dict[@"attrs"];

        [self trackPurchaseWithPrice:price
                            currency:currency
                             success:success
                            itemName:itemName
                            itemType:itemType
                              itemId:itemId
                    customAttributes:attrs];
        return @"";
    }
                         tag:k__crashlytics_track_purchase];

    [bridge_ registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 2, @"...");

        NSString* name = dict[@"name"];
        NSDictionary* attrs = dict[@"attrs"];

        [self trackCustomEvent:name customAttributes:attrs];
        return @"";
    }
                         tag:k__crashlytics_track_custom_event];

    [bridge_ registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSAssert([dict count] == 2, @"...");

        NSString* method = dict[@"method"];
        NSDictionary* attrs = dict[@"attrs"];

        [self trackInviteWithMethod:method customAttributes:attrs];
        return @"";
    }
                         tag:k__crashlytics_track_invite];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__crashlytics_cause_crash];
    [bridge_ deregisterHandler:k__crashlytics_cause_exception];
    [bridge_ deregisterHandler:k__crashlytics_log];
    [bridge_ deregisterHandler:k__crashlytics_set_log_level];
    [bridge_ deregisterHandler:k__crashlytics_set_string];
    [bridge_ deregisterHandler:k__crashlytics_set_bool];
    [bridge_ deregisterHandler:k__crashlytics_set_int];
    [bridge_ deregisterHandler:k__crashlytics_set_user_identifier];
    [bridge_ deregisterHandler:k__crashlytics_set_user_name];
    [bridge_ deregisterHandler:k__crashlytics_set_user_email];
    [bridge_ deregisterHandler:k__crashlytics_track_level_start];
    [bridge_ deregisterHandler:k__crashlytics_track_level_end];
    [bridge_ deregisterHandler:k__crashlytics_track_purchase];
    [bridge_ deregisterHandler:k__crashlytics_track_invite];
    [bridge_ deregisterHandler:k__crashlytics_track_custom_event];
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

- (void)trackLevelStart:(NSString* _Nonnull)name
       customAttributes:(NSDictionary* _Nonnull)attrs {
    [Answers logLevelStart:name customAttributes:attrs];
}

- (void)trackLevelEnd:(NSString* _Nonnull)name
                score:(NSNumber* _Nonnull)score
              success:(NSNumber* _Nonnull)success
     customAttributes:(NSDictionary* _Nonnull)attrs {
    [Answers logLevelEnd:name
                   score:score
                 success:success
        customAttributes:attrs];
}

- (void)trackPurchaseWithPrice:(NSNumber* _Nonnull)price
                      currency:(NSString* _Nonnull)currency
                       success:(NSNumber* _Nonnull)success
                      itemName:(NSString* _Nonnull)itemName
                      itemType:(NSString* _Nonnull)itemType
                        itemId:(NSString* _Nonnull)itemId
              customAttributes:(NSDictionary<NSString*, id>* _Nonnull)attrs {
    [Answers
        logPurchaseWithPrice:[NSDecimalNumber
                                 decimalNumberWithDecimal:[price decimalValue]]
                    currency:currency
                     success:success
                    itemName:itemName
                    itemType:itemType
                      itemId:itemId
            customAttributes:attrs];
}

- (void)trackCustomEvent:(NSString* _Nonnull)eventName
        customAttributes:(NSDictionary<NSString*, id>* _Nonnull)attrs {
    [Answers logCustomEventWithName:eventName customAttributes:attrs];
}

- (void)trackInviteWithMethod:(NSString*)method
             customAttributes:(NSDictionary<NSString*, id>* _Nonnull)attrs {
    [Answers logInviteWithMethod:method customAttributes:attrs];
}
@end
