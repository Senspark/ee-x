//
//  EEKeyValDatabase.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#import "EEKeyValueStorage.h"

@implementation EEKeyValueStorage

+ (instancetype)getInstance {
    static EEKeyValueStorage* sharedInstance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[EEKeyValueStorage alloc] init];
    });
    return sharedInstance;
}

- (id)initWithName:(NSString*)storageName andSecret:(NSString*)secret {
    @synchronized(self) {
        self = [super init];
        if (self == nil) {
            return nil;
        }
        keeva_ = [[Keeva alloc] initWithName:storageName andSecret:secret];
        return self;
    }
}

@end
