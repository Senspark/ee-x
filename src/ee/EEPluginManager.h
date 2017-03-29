//
//  EEPluginManager.hpp
//  ee_core
//
//  Created by Zinge on 3/29/17.
//
//

#import <Foundation/Foundation.h>

@interface EEPluginManager : NSObject {
    NSMutableDictionary* plugins;
}

+ (id _Null_unspecified)getInstance;

- (void)addPlugin:(NSString* _Nonnull)pluginName;

- (void)removePlugin:(NSString* _Nonnull)pluginName;

@end
