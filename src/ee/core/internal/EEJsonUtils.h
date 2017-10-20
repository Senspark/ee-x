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
+ (NSString*)convertArrayToString:(NSArray*)arr;

/// Converts the specified dictionary to a JSON formatted string.
+ (NSString*)convertDictionaryToString:(NSDictionary*)dict;

/// Converts a JSON formatted string to a dictionary.
+ (NSMutableDictionary*)convertStringToDictionary:(NSString*)str;

@end
