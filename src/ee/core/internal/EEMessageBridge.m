//
//  EEPluginManager.cpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"

@implementation EEMessageBridge

+ (instancetype)getInstance {
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
    handlers = [[NSMutableDictionary alloc] init];
    return self;
}

- (void)dealloc {
    [handlers release];
    handlers = nil;
    [super dealloc];
}

- (BOOL)registerHandler:(EEMessageHandler _Nonnull)handler
                    tag:(NSString* _Nonnull)tag {
    if ([handlers objectForKey:tag] != nil) {
        NSAssert(NO, @"...");
        return NO;
    }
    [handlers setValue:[[handler copy] autorelease] forKey:tag];
    return YES;
}

- (BOOL)registerHandler:(NSString* _Nonnull)tag
               callback:(EEMessageHandler _Nonnull)handler {
    return [self registerHandler:handler tag:tag];
}

- (BOOL)deregisterHandler:(NSString* _Nonnull)tag {
    if ([handlers objectForKey:tag] == nil) {
        NSAssert(NO, @"...");
        return NO;
    }
    [handlers removeObjectForKey:tag];
    return YES;
}

@end
