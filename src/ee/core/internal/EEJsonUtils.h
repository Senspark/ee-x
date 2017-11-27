//
//  __EEJsonUtils.hpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import <Foundation/Foundation.h>

/// Utility class to convert between @c NSString and @c NSDictionary.
@interface EEJsonUtils : NSObject

/// Converts the specified array to a JSON formatted string.
+ (NSString* _Nullable)convertArrayToString:(NSArray* _Nonnull)arr;

/// Converts the specified dictionary to a JSON formatted string.
+ (NSString* _Nullable)convertDictionaryToString:(NSDictionary* _Nonnull)dict;

+ (NSMutableArray* _Nullable)convertStringToArray:(NSString* _Nonnull)str;

/// Converts a JSON formatted string to a dictionary.
+ (NSMutableDictionary* _Nullable)convertStringToDictionary:
    (NSString* _Nonnull)str;

@end
