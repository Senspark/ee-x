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

#import <Foundation/Foundation.h>

/**
 * An Encryptor that uses AES to encrypt data.
 */
@interface KeevaEncryptor : NSObject

/**
 * Returns the key used for encrypting/decrypting values during transformation.
 */
//+ (NSString*)key;

+ (NSString *)encryptString:(NSString *)data;

+ (NSString *)encryptString:(NSString *)data withKey:(NSString *)key;

+ (NSString *)decryptToString:(NSString *)data;

+ (NSString *)decryptToString:(NSString *)data withKey:(NSString *)key;

+ (NSString *)encryptNumber:(NSNumber *)data;

+ (NSString *)encryptNumber:(NSNumber *)data withKey:(NSString *)key;

+ (NSNumber *)decryptToNumber:(NSString *)data;

+ (NSNumber *)decryptToNumber:(NSString *)data withKey:(NSString *)key;

+ (NSString *)encryptBoolean:(BOOL)data;

+ (NSString *)encryptBoolean:(BOOL)data withKey:(NSString *)key;

+ (BOOL)decryptToBoolean:(NSString *)data;

+ (BOOL)decryptToBoolean:(NSString *)data withKey:(NSString *)key;

@end

