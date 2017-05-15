//
//  EEKeyValDatabase.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#import <Foundation/Foundation.h>

@class Keeva;

@interface EEKeyValueStorage : NSObject {
    Keeva* keeva_;
}

+ (instancetype)getInstance;

- (id)initWithName:(NSString*)storageName andSecret:(NSString*)secret;

@end
