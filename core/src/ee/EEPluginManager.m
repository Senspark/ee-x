//
//  EEPluginManager.cpp
//  ee_core
//
//  Created by Zinge on 3/29/17.
//
//

#import "EEPluginManager.h"

@implementation EEPluginManager

+ (id)getInstance {
    static EEPluginManager* sharedInstance = nil;
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
    plugins = [[NSMutableDictionary alloc] init];
    return self;
}

- (void)dealloc {
    [plugins release];
    plugins = nil;
    [super dealloc];
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
