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
+ (NSString* _Nonnull)convertArrayToString:(NSArray* _Nonnull)arr;

/// Converts the specified dictionary to a JSON formatted string.
+ (NSString* _Nonnull)convertDictionaryToString:(NSDictionary* _Nonnull)dict;

+ (NSMutableArray* _Nonnull)convertStringToArray:(NSString* _Nonnull)str;

/// Converts a JSON formatted string to a dictionary.
+ (NSMutableDictionary* _Nonnull)convertStringToDictionary:
    (NSString* _Nonnull)str;

@end
