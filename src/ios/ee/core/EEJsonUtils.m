//
//  __EEJsonUtils.cpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#if __has_include("ee/EEJsonUtils.h")
// Build dynamic framework.
#import "ee/EEJsonUtils.h"
#else
#import "ee/core/EEJsonUtils.h"
#endif

@implementation EEJsonUtils

+ (NSString* _Nullable)convertObjectToString:(id _Nonnull)object {
    NSError* error;
    NSUInteger options = NSJSONWritingPrettyPrinted;
    NSData* data = [NSJSONSerialization dataWithJSONObject:object
                                                   options:options
                                                     error:&error];
    if (data == nil) {
        NSLog(@"%@", error);
        return nil;
    }
    return [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]
        autorelease];
}

+ (id _Nullable)convertStringToObject:(NSString* _Nonnull)str {
    NSError* error;
    NSData* data = [str dataUsingEncoding:NSUTF8StringEncoding];
    NSUInteger options =
        (NSJSONReadingMutableContainers | NSJSONReadingMutableLeaves);
    id object = [NSJSONSerialization JSONObjectWithData:data
                                                options:options
                                                  error:&error];
    if (object == nil) {
        NSLog(@"%@", error);
        return nil;
    }
    return object;
}

+ (NSString* _Nonnull)convertArrayToString:(NSArray* _Nonnull)arr {
    NSString* str = [self convertObjectToString:arr];
    NSAssert(str != nil,
             @"Unexpected error when converting NSArray to NSString");
    return str;
}

+ (NSString* _Nonnull)convertDictionaryToString:(NSDictionary* _Nonnull)dict {
    NSString* str = [self convertObjectToString:dict];
    NSAssert(str != nil,
             @"Unexpected error when converting NSDictionary to NSString");
    return str;
}

+ (NSMutableArray* _Nonnull)convertStringToArray:(NSString* _Nonnull)str {
    id object = [self convertStringToObject:str];
    NSAssert(object != nil,
             @"Unexpected error when convert NSString to NSArray");
    NSAssert([object isKindOfClass:[NSMutableArray class]],
             @"Converted object is not NSArray");
    return object;
}

+ (NSMutableDictionary* _Nonnull)convertStringToDictionary:(NSString*)str {
    id object = [self convertStringToObject:str];
    NSAssert(object != nil,
             @"Unexpected error when convert NSString to NSDictionary");
    NSAssert([object isKindOfClass:[NSMutableDictionary class]],
             @"Converted object is not NSDictionary");
    return object;
}

@end
