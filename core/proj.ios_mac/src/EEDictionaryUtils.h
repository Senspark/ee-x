//
//  EEPluginDictionary.hpp
//  roll-eat
//
//  Created by Zinge on 5/18/16.
//
//

#import <Foundation/Foundation.h>

@interface EEDictionaryUtils : NSObject

// clang-format off

+ (NSString* _Nonnull) emptyResult;

+ (void) setError:(NSError* _Nullable) error
          forDict:(NSMutableDictionary* _Nonnull) dict;

+ (void) setResult:(id _Nullable) result
           forDict:(NSMutableDictionary* _Nonnull) dict;

// clang-format on

@end
