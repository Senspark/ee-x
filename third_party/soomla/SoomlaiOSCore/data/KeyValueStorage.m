/*
 Copyright (C) 2012-2014 Soomla Inc.
 
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

#import "KeyValueStorage.h"
#import "KeyValDatabase.h"
#import "SoomlaConfig.h"
#import "KeevaConfig.h"
#import "SoomlaUtils.h"
#import "Soomla.h"
#import "Keeva.h"

@interface KeyValueStorage()

@property(nonatomic) Keeva *keeva;

@end

@implementation KeyValueStorage

+ (NSString*)getValueForKey:(NSString*)key {
    return [[self getDefaultStorage] getValueForKey:key];
}

+ (void)setValue:(NSString*)val forKey:(NSString*)key {
    [[self getDefaultStorage] setValue:val forKey:key];
}

+ (void)deleteValueForKey:(NSString*)key {
    [[self getDefaultStorage] deleteValueForKey:key];
}

+ (NSDictionary*)getKeysValuesForNonEncryptedQuery:(NSString*)query {
    return [[self getDefaultStorage] getKeysValuesForNonEncryptedQuery:query];
}

+ (NSArray*)getValuesForNonEncryptedQuery:(NSString*)query {
    return [[self getDefaultStorage] getValuesForNonEncryptedQuery:query];
}

+ (NSArray*)getValuesForNonEncryptedQuery:(NSString*)query withLimit:(int)limit {
    return [[self getDefaultStorage] getValuesForNonEncryptedQuery:query withLimit:limit];
}

+ (NSString*)getOneForNonEncryptedQuery:(NSString*)query {
    return [[self getDefaultStorage] getOneForNonEncryptedQuery:query];
}

+ (int)getCountForNonEncryptedQuery:(NSString*)query {
    return [[self getDefaultStorage] getCountForNonEncryptedQuery:query];
}

+ (NSString*)getValueForNonEncryptedKey:(NSString*)key {
    return [[self getDefaultStorage] getValueForNonEncryptedKey:key];
}

+ (NSArray *)getEncryptedKeys {
    return [[self getDefaultStorage] getEncryptedKeys];
}

+ (void)setValue:(NSString*)val forNonEncryptedKey:(NSString*)key {
    [[self getDefaultStorage] setValue:val forNonEncryptedKey:key];
}

+ (void)deleteValueForNonEncryptedKey:(NSString*)key {
    [[self getDefaultStorage] deleteValueForNonEncryptedKey:key];
}

+ (void)purge {
    [[self getDefaultStorage] purge];
}

+ (KeyValueStorage*)getDefaultStorage {
    static KeyValueStorage* defaultStorage;
    if (!defaultStorage) {
        defaultStorage = [[KeyValueStorage alloc] initWithName:SOOMLA_DATABASE_NAME andSecret:SOOMLA_SECRET];
    }
    
    return defaultStorage;
}


- (id)initWithName:(NSString*)storageName andSecret:(NSString*)secret {
    @synchronized(self) {
        if (self = [super init]) {
            if (!secret || [secret length]==0) {
                [NSException raise:@"Invalid secret value" format:@"You must initialize KeyValueStorage with a secret. storageName: %@", storageName];
            }
            self.keeva = [[Keeva alloc] initWithName:storageName andSecret:secret];
            KEEVA_DEBUG_LOG = DEBUG_LOG;
        }
        return self;
    }
}

- (NSString*)getValueForKey:(NSString*)key {
    return [self.keeva getValueForKey:key];
}

- (void)setValue:(NSString*)val forKey:(NSString*)key {
    [self.keeva setValue:val forKey:key];
}

- (void)deleteValueForKey:(NSString*)key {
    [self.keeva deleteValueForKey:key];
}

- (NSDictionary*)getKeysValuesForNonEncryptedQuery:(NSString*)query {
    return [self.keeva getKeysValuesForNonEncryptedQuery:query];
}
- (NSArray*)getValuesForNonEncryptedQuery:(NSString*)query {
    return [self.keeva getValuesForNonEncryptedQuery:query];
}

- (NSArray*)getValuesForNonEncryptedQuery:(NSString*)query withLimit:(int)limit {
    return [self.keeva getValuesForNonEncryptedQuery:query withLimit:limit];
}

- (NSString*)getOneForNonEncryptedQuery:(NSString*)query {
    return [self.keeva getOneForNonEncryptedQuery:query];
}

- (int)getCountForNonEncryptedQuery:(NSString*)query {
    return [self.keeva getCountForNonEncryptedQuery:query];
}

- (NSString*)getValueForNonEncryptedKey:(NSString*)key {
    return [self.keeva getValueForNonEncryptedKey:key];
}

- (NSArray *)getEncryptedKeys {
    return [self.keeva getEncryptedKeys];
}

- (void)setValue:(NSString*)val forNonEncryptedKey:(NSString*)key {
    [self.keeva setValue:val forNonEncryptedKey:key];
}

- (void)deleteValueForNonEncryptedKey:(NSString*)key {
    [self.keeva deleteValueForNonEncryptedKey:key];
}

- (void)purge {
    [self.keeva purge];
}

@end
