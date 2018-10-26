/*
 Copyright (C) 2012-2016 Soomla Inc.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#import "KeevaEncryptor.h"
#import "KeevaConfig.h"
#import <CommonCrypto/CommonCryptor.h>
#import "FBEncryptorAES.h"
#import "Keeva.h"
#import "KeevaUtils.h"

@implementation KeevaEncryptor


/*
 * The encryption key is comprised of the SOOMLA secret and a unique global identifier for the specific application.
 * NOTE: change the custom secret in KeevaConfig.h.
 */
//+ (NSString*)key
//{
//    NSString* sec = SOOMLA_SECRET;
//    if (!sec || [sec length]==0) {
//        LogError(@"KEEVA KeevaEncryptor", @"SOOMLA sercet not provided. This is serious and can lead to multiple errors!");
//    }
//    return [KeevaUtils keyFromSecret:sec];
//}

+ (NSString *)encryptString:(NSString *)data{
    @synchronized(self) {
        return [self encryptString:data withKey:nil];
    }
}

+ (NSString *)encryptString:(NSString *)data withKey:(NSString *)key{
    @synchronized(self) {
        return [FBEncryptorAES encryptBase64String:data
                                         keyString:key
                                     separateLines:NO];
    }
}

+ (NSString *)decryptToString:(NSString *)data{
    @synchronized(self) {
        return [self decryptToString:data withKey:nil];
    }
}

+ (NSString *)decryptToString:(NSString *)data withKey:(NSString *)key{
    @synchronized(self) {
        return [FBEncryptorAES decryptBase64String:data
                                         keyString:key];
    }
}

+ (NSString *)encryptNumber:(NSNumber *)data{
    @synchronized(self) {
        return [self encryptString:[data stringValue]];
    }
}

+ (NSString *)encryptNumber:(NSNumber *)data withKey:(NSString *)key{
    @synchronized(self) {
        return [self encryptString:[data stringValue] withKey:key];
    }
}

+ (NSNumber *)decryptToNumber:(NSString *)data{
    @synchronized(self) {
        data = [self decryptToString:data];
        NSNumberFormatter * f = [[NSNumberFormatter alloc] init];
        [f setNumberStyle:NSNumberFormatterDecimalStyle];
        return [f numberFromString:data];
    }
}

+ (NSNumber *)decryptToNumber:(NSString *)data withKey:(NSString *)key{
    @synchronized(self) {
        data = [self decryptToString:data withKey:key];
        NSNumberFormatter * f = [[NSNumberFormatter alloc] init];
        [f setNumberStyle:NSNumberFormatterDecimalStyle];
        return [f numberFromString:data];
    }
}

+ (NSString *)encryptBoolean:(BOOL)data{
    @synchronized(self) {
        return [self encryptString:[[NSNumber numberWithBool:data] stringValue]];
    }
}

+ (NSString *)encryptBoolean:(BOOL)data withKey:(NSString *)key{
    @synchronized(self) {
        return [self encryptString:[[NSNumber numberWithBool:data] stringValue] withKey:key];
    }
}

+ (BOOL)decryptToBoolean:(NSString *)data{
    @synchronized(self) {
        data = [self decryptToString:data];
        NSNumber *res = [NSNumber numberWithInt:[data intValue]];
        return [res boolValue];
    }
}

+ (BOOL)decryptToBoolean:(NSString *)data withKey:(NSString *)key{
    @synchronized(self) {
        data = [self decryptToString:data withKey:key];
        NSNumber *res = [NSNumber numberWithInt:[data intValue]];
        return [res boolValue];
    }
}

@end


