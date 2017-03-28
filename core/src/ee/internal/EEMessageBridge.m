//
//  EEPluginManager.cpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import "ee/internal/EEMessageBridge.h"
#import "ee/internal/EEJsonUtils.h"
#import "ee/internal/EEDictionaryUtils.h"

@implementation EEMessageBridge

+ (id)getInstance {
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

- (NSString* _Nonnull)call:(NSString* _Nonnull)tag msg:(NSString* _Nonnull)msg {
    EEMessageHandler handler = [handlers objectForKey:tag];
    if (handler == nil) {
    }
    return handler(msg);
}

- (void)registerHandler:(EEMessageHandler _Nonnull)handler
                    tag:(NSString* _Nonnull)tag {
    [handlers setValue:[[handler copy] autorelease] forKey:tag];
}

- (void)deregisterHandler:(NSString*)tag {
    [handlers removeObjectForKey:tag];
}

@end
