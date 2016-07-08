//
//  EEPluginManager.hpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import <Foundation/Foundation.h>

@class EEPluginProtocol;

@interface EEPluginManager : NSObject

// clang-format off

+ (EEPluginProtocol* _Null_unspecified) getPluginByName:(NSString* _Nonnull) pluginName;

+ (NSString* _Nonnull) callNative:(NSString* _Nonnull) pluginName
                           method:(NSString* _Nonnull) methodName
                          message:(NSString* _Nullable) json
                       callbackId:(NSNumber* _Nullable) callbackId;

// clang-format on

@end