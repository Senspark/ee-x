//
//  EEPluginManager.cpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import "ee/internal/EEPluginManager.h"
#import "ee/internal/EEJsonUtils.h"
#import "ee/internal/EEDictionaryUtils.h"
#import "ee/internal/EEPluginProtocol.h"
#import "ee/internal/EEPluginManager.h"

@implementation EEPluginManager

// clang-format off

+ (EEPluginProtocol* _Null_unspecified) getPluginByName:(NSString* _Nonnull) pluginName {
    static NSMutableDictionary* plugins;
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        plugins = [[NSMutableDictionary alloc] init];
    });
    
    if ([plugins objectForKey:pluginName] == nil) {
        Class clazz = NSClassFromString([NSString stringWithFormat:@"EE%@", pluginName]);
        [plugins setValue:[[clazz alloc] init] forKey:pluginName];
    }
    
    return [plugins objectForKey:pluginName];
}

+ (NSString* _Nonnull) callNative:(NSString* _Nonnull) pluginName
                           method:(NSString* _Nonnull) methodName
                          message:(NSString* _Nullable) json
                       callbackId:(NSNumber* _Nullable) callbackId {
    EEPluginProtocol* plugin = [EEPluginManager getPluginByName:pluginName];
    
    NSMutableArray* params = [NSMutableArray array];
    if (json != nil) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:json];
        if (dict == nil) {
            @throw [NSException exceptionWithName:NSInvalidArgumentException
                                           reason:@"Invalid JSON string"
                                         userInfo:nil];
        }
        [params addObject:dict];
    }
    if (callbackId != nil) {
        [params addObject:(NSNumber* _Nonnull) callbackId];
    }
    
    NSString* selectorName;
    if ([params count] == 0) {
        selectorName = [NSString stringWithFormat:@"%s", [methodName UTF8String]];
    } else if ([params count] == 1) {
        methodName = [NSString stringWithFormat:@"%s:", [methodName UTF8String]];
    } else if ([params count] == 2) {
        methodName = [NSString stringWithFormat:@"%s:callbackId:", [methodName UTF8String]];
    } else {
        @throw [NSException exceptionWithName:NSInvalidArgumentException
                                       reason:@"Invalid number of arguments"
                                     userInfo:nil];
    }
    
    SEL selector = NSSelectorFromString(methodName);
    id result = nil;
    if ([plugin respondsToSelector:selector]) {
        NSMethodSignature* signature = [plugin methodSignatureForSelector:selector];
        NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
        [invocation setSelector:selector];
        [invocation setTarget:plugin];
        for (NSUInteger i = 0; i < [params count]; ++i) {
            id param = [params objectAtIndex:i];
            [invocation setArgument:&param atIndex:(NSInteger) i + 2];
        }
        [invocation invoke];
        
        const char* resultType = [signature methodReturnType];
        if (strcmp(resultType, @encode(void)) != 0) {
            [invocation getReturnValue:&result];
        }
    } else {
        @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                       reason:@"Missing selector"
                                     userInfo:nil];
    }
    
    if (result != nil) {
        NSAssert([result isKindOfClass:[NSString class]], @"...");
        return result;
    }
    
    return [EEDictionaryUtils emptyResult];
}

// clang-format on

@end
