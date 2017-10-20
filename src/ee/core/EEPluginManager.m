//
//  EEPluginManager.cpp
//  ee_core
//
//  Created by Zinge on 3/29/17.
//
//

#import "ee/core/EEPluginManager.h"

@interface EEPluginManager () {
    NSMutableDictionary* plugins_;
}

@end

@implementation EEPluginManager

+ (instancetype _Nonnull)getInstance {
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
    plugins_ = [[NSMutableDictionary alloc] init];
    return self;
}

- (void)dealloc {
    [plugins_ release];
    plugins_ = nil;
    [super dealloc];
}

- (void)addPlugin:(NSString* _Nonnull)pluginName {
    if ([plugins_ objectForKey:pluginName] != nil) {
        NSAssert(NO, @"...");
        return;
    }

    Class clazz =
        NSClassFromString([NSString stringWithFormat:@"EE%@", pluginName]);
    [plugins_ setObject:[[[clazz alloc] init] autorelease] forKey:pluginName];
}

- (void)removePlugin:(NSString* _Nonnull)pluginName {
    if ([plugins_ objectForKey:pluginName] == nil) {
        NSAssert(NO, @"...");
        return;
    }
    [plugins_ removeObjectForKey:pluginName];
}

@end
