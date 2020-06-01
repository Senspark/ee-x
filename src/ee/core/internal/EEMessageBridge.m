//
//  EEPluginManager.cpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import "ee/core/internal/EEMessageBridge.h"

#import "ee/core/internal/EEJsonUtils.h"

@implementation EEMessageBridge {
    /// Registered handlers.
    NSMutableDictionary* handlers_;
}

+ (instancetype _Nonnull)getInstance {
    static EEMessageBridge* sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

- (id)init {
    self = [super init];
    if (self == nil) {
        return nil;
    }
    handlers_ = [[NSMutableDictionary alloc] init];
    return self;
}

- (void)dealloc {
    [handlers_ release];
    handlers_ = nil;
    [super dealloc];
}

- (BOOL)isHandlerRegistered:(NSString* _Nonnull)tag {
    return [handlers_ objectForKey:tag] != nil;
}

- (BOOL)registerHandler:(EEMessageHandler _Nonnull)handler
                    tag:(NSString* _Nonnull)tag {
    if ([self isHandlerRegistered:tag]) {
        NSString* message = [NSString
            stringWithFormat:@"A handler with tag %@ already exists", tag];
        NSAssert(NO, message);
        return NO;
    }
    [handlers_ setValue:[[handler copy] autorelease] forKey:tag];
    return YES;
}

- (BOOL)registerHandler:(NSString* _Nonnull)tag
               callback:(EEMessageHandler _Nonnull)handler {
    return [self registerHandler:handler tag:tag];
}

- (BOOL)registerAsyncHandler:(NSString* _Nonnull)tag
                    callback:(EEMessageAsyncHandler _Nonnull)handler {
    return [self
        registerHandler:tag
               callback:^(NSString* _Nonnull message) {
                   NSDictionary* dict =
                       [EEJsonUtils convertStringToDictionary:message];

                   NSString* callbackTag = (NSString*)dict[@"callback_tag"];
                   NSString* originalMessage = (NSString*)dict[@"message"];

                   handler(
                       originalMessage, ^(NSString* _Nonnull callbackMessage) {
                           [self callCpp:callbackTag message:callbackMessage];
                       });
                   return @"";
               }];
}

- (BOOL)deregisterHandler:(NSString* _Nonnull)tag {
    if (![self isHandlerRegistered:tag]) {
        NSString* message = [NSString
            stringWithFormat:@"A handler with tag %@ doesn't exist", tag];
        NSAssert(NO, message);
        return NO;
    }
    [handlers_ removeObjectForKey:tag];
    return YES;
}

- (NSString* _Nonnull)call:(NSString* _Nonnull)tag
                   message:(NSString* _Nonnull)message {
    EEMessageHandler handler = [handlers_ objectForKey:tag];
    if (handler == nil) {
        NSString* errorMessage = [NSString
            stringWithFormat:@"A handler with tag %@ doesn't exist", tag];
        NSAssert(NO, errorMessage);
        return @"";
    }
    return handler(message);
}

- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag {
    return [self callCppInternal:tag message:@""];
}

- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag
                      message:(NSString* _Nonnull)message {
    return [self callCppInternal:tag message:message];
}

@end
