//
//  EEPluginManager.cpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import "ee/core/EEMessageBridge.h"

@interface EEMessageBridge () {
    /// Registered handlers.
    NSMutableDictionary* handlers_;
}

@end

@implementation EEMessageBridge

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

@end
