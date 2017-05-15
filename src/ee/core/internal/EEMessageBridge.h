//
//  EEPluginManager.hpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import <Foundation/Foundation.h>

typedef NSString* _Nonnull (^EEMessageHandler)(NSString* _Nonnull msg);

@interface EEMessageBridge : NSObject {
    NSMutableDictionary* handlers;
}

+ (id _Null_unspecified)getInstance;

- (NSString* _Nonnull)call:(NSString* _Nonnull)tag msg:(NSString* _Nonnull)msg;

- (void)registerHandler:(EEMessageHandler _Nonnull)handler
                    tag:(NSString* _Nonnull)tag;

- (void)deregisterHandler:(NSString* _Nonnull)tag;

@end

@interface EEMessageBridge (Cpp)

- (NSString* _Nonnull)callCpp:(NSString* _Nonnull)tag
                          msg:(NSString* _Nonnull)msg;

@end
