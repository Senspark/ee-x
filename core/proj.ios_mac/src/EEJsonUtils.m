//
//  __EEJsonUtils.cpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import "EEJsonUtils.h"

@implementation EEJsonUtils

// clang-format off

+ (NSString*) convertDictionaryToString:(NSDictionary*) dict {
    NSError* error;
    NSUInteger options = NSJSONWritingPrettyPrinted;
    NSData* data = [NSJSONSerialization dataWithJSONObject:dict
                                                   options:options
                                                     error:&error];
    if (data == nil) {
        NSLog(@"%@", error);
        NSCAssert(NO, @"...");
        return nil;
    }
    return [[[NSString alloc] initWithData:data
                                  encoding:NSUTF8StringEncoding] autorelease];
}

+ (NSMutableDictionary*) convertStringToDictionary:(NSString*) str {
    NSError* error;
    NSData* data = [str dataUsingEncoding:NSUTF8StringEncoding];
    NSUInteger options = (NSJSONReadingMutableContainers |
                          NSJSONReadingMutableLeaves);
    id object = [NSJSONSerialization JSONObjectWithData:data
                                                options:options
                                                  error:&error];
    if (object == nil) {
        NSLog(@"%@", error);
        NSCAssert(NO, @"...");
        return nil;
    }
    NSCAssert([object isKindOfClass:[NSMutableDictionary class]], @"...");
    return object;
}

// clang-format on

@end