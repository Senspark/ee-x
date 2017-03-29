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

NSString* const k__plugin_manager_add_plugin = @"__plugin_manager_add_plugin";
NSString* const k__plugin_manager_remove_plugin =
    @"__plugin_manager_remove_plugin";

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
    plugins = [[NSMutableDictionary alloc] init];

    [self registerHandler:^(NSString* msg) {
        [self addPlugin:msg];
        return [EEDictionaryUtils emptyResult];
    } tag:k__plugin_manager_add_plugin];

    [self registerHandler:^(NSString* msg) {
        [self removePlugin:msg];
        return [EEDictionaryUtils emptyResult];
    } tag:k__plugin_manager_remove_plugin];

    return self;
}

- (void)dealloc {
    [plugins release];
    plugins = nil;

    [self deregisterHandler:k__plugin_manager_add_plugin];
    [self deregisterHandler:k__plugin_manager_remove_plugin];

    [handlers release];
    handlers = nil;
    [super dealloc];
}

- (NSString* _Nonnull)call:(NSString* _Nonnull)tag msg:(NSString* _Nonnull)msg {
    EEMessageHandler handler = [handlers objectForKey:tag];
    if (handler == nil) {
        NSAssert(NO, @"...");
        return [EEDictionaryUtils emptyResult];
    }
    return handler(msg);
}

- (void)registerHandler:(EEMessageHandler _Nonnull)handler
                    tag:(NSString* _Nonnull)tag {
    if ([handlers objectForKey:tag] != nil) {
        NSAssert(NO, @"...");
        return;
    }
    [handlers setValue:[[handler copy] autorelease] forKey:tag];
}

- (void)deregisterHandler:(NSString*)tag {
    if ([handlers objectForKey:tag] == nil) {
        NSAssert(NO, @"...");
        return;
    }
    [handlers removeObjectForKey:tag];
}

- (void)addPlugin:(NSString* _Nonnull)pluginName {
    if ([plugins objectForKey:pluginName] != nil) {
        NSAssert(NO, @"...");
        return;
    }

    Class clazz =
        NSClassFromString([NSString stringWithFormat:@"EE%@", pluginName]);
    [plugins setObject:[[[clazz alloc] init] autorelease] forKey:pluginName];
}

- (void)removePlugin:(NSString* _Nonnull)pluginName {
    if ([plugins objectForKey:pluginName] == nil) {
        NSAssert(NO, @"...");
        return;
    }
    [plugins removeObjectForKey:pluginName];
}

@end
