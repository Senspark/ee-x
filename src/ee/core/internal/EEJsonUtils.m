//
//  __EEJsonUtils.cpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import "ee/core/internal/EEJsonUtils.h"

@implementation EEJsonUtils

+ (NSString* _Nullable)convertObjectToString:(id _Nonnull)object {
    NSError* error;
    NSUInteger options = NSJSONWritingPrettyPrinted;
    NSData* data = [NSJSONSerialization dataWithJSONObject:object
                                                   options:options
                                                     error:&error];
    if (data == nil) {
        NSLog(@"%@", error);
        NSAssert(NO, @"Unexpected error when converting an object to NSString");
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
        NSAssert(NO, @"Unexpected error when convert NSString to an object");
        return nil;
    }
    return object;
}

+ (NSString* _Nullable)convertArrayToString:(NSArray* _Nonnull)arr {
    return [self convertObjectToString:arr];
}

+ (NSString* _Nullable)convertDictionaryToString:(NSDictionary* _Nonnull)dict {
    return [self convertObjectToString:dict];
}

+ (NSMutableArray* _Nullable)convertStringToArray:(NSString* _Nonnull)str {
    id object = [self convertStringToObject:str];
    NSAssert([object isKindOfClass:[NSMutableArray class]],
             @"Converted object is not NSArray");
    return object;
}

+ (NSMutableDictionary* _Nullable)convertStringToDictionary:(NSString*)str {
    id object = [self convertStringToObject:str];
    NSAssert([object isKindOfClass:[NSMutableDictionary class]],
             @"Converted object is not NSDictionary");
    return object;
}

@end
