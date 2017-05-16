//
//  EEPluginDictionary.hpp
//  roll-eat
//
//  Created by Zinge on 5/18/16.
//
//

#import <Foundation/Foundation.h>

/// Utility class to manipulate dictionary.
@interface EEDictionaryUtils : NSObject

/// Creates an empty JSON formatted string.
+ (NSString* _Nonnull)emptyResult;

+ (NSDictionary* _Nonnull)createDictWithError:(NSError* _Nullable)error;

/// Adds the specified error to the specified dictionary.
+ (void)setError:(NSError* _Nullable)error
         forDict:(NSMutableDictionary* _Nonnull)dict;

/// Adds the result to the specified dictionary.
+ (void)setResult:(id _Nullable)result
          forDict:(NSMutableDictionary* _Nonnull)dict;

@end
