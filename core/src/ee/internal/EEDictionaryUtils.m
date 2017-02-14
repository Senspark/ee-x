//
//  EEPluginDictionary.cpp
//  roll-eat
//
//  Created by Zinge on 5/18/16.
//
//

#import "ee/internal/EEDictionaryUtils.h"
#import "ee/internal/EEJsonUtils.h"

@implementation EEDictionaryUtils

// clang-format off

+ (NSString* _Nonnull) emptyResult {
    return [EEJsonUtils convertDictionaryToString:@{}];
}

+ (void) setError:(NSError* _Nullable) error
          forDict:(NSMutableDictionary* _Nonnull) dict {
    if (error != nil) {
        [dict setObject:@([error code]) forKey:@"errorCode"];
        
        NSString* description = [error localizedDescription];
        if (description != nil) {
            [dict setObject:description forKey:@"errorDescription"];
        }
    }
}

+ (void) setResult:(id _Nullable) result
           forDict:(NSMutableDictionary* _Nonnull) dict {
    if (result != nil) {
        [dict setObject:(id _Nonnull) result forKey:@"result"];
    }
}

// clang-format on

@end
