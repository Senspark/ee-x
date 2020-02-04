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

#import "Keeva.h"
#import "KeevaDatabase.h"
#import "KeevaEncryptor.h"
#import "KeevaConfig.h"
#import "KeevaUtils.h"

@interface Keeva()

@property(nonatomic,retain) KeevaDatabase *kvDatabase;
@property(nonatomic,retain) NSString *storageName;
@property(nonatomic,retain) NSString *key;

@end

@implementation Keeva

- (id)initWithName:(NSString*)storageName andSecret:(NSString*)secret {
    @synchronized(self) {
        if (self = [super init]) {
            if (!secret || [secret length]==0) {
                [NSException raise:@"Invalid secret value" format:@"You must initialize Keeva with a secret. storageName: %@", storageName];
            }
            self.storageName = storageName;
            self.key = [KeevaUtils keyFromSecret:secret];
            self.kvDatabase = [[KeevaDatabase alloc] initWithName:storageName];
        }
        return self;
    }
}

- (NSString*)getValueForKey:(NSString*)key {
    key = [KeevaEncryptor encryptString:key withKey:self.key];
    NSString* val = [self.kvDatabase getValForKey:key];
    if (val && [val length]>0){
        return [KeevaEncryptor decryptToString:val withKey:self.key];
    }
    
    return NULL;
}

- (void)setValue:(NSString*)val forKey:(NSString*)key {
    key = [KeevaEncryptor encryptString:key withKey:self.key];
    [self.kvDatabase setVal:[KeevaEncryptor encryptString:val withKey:self.key] forKey:key];
}

- (void)deleteValueForKey:(NSString*)key {
    key = [KeevaEncryptor encryptString:key withKey:self.key];
    [self.kvDatabase deleteKeyValWithKey:key];
}

- (NSDictionary*)getKeysValuesForNonEncryptedQuery:(NSString*)query {
    NSDictionary* dbResults = [self.kvDatabase getKeysValsForQuery:query];
    NSMutableDictionary* results = [NSMutableDictionary dictionary];
    NSArray* keys = [dbResults allKeys];
    for (NSString* key in keys) {
        NSString* val = dbResults[key];
        if (val && [val length]>0){
            NSString* valDec = [KeevaEncryptor decryptToString:val withKey:self.key];
            if (valDec && [valDec length]>0){
                [results setObject:valDec forKey:key];
            }
        }
    }
    
    return results;
}
- (NSArray*)getValuesForNonEncryptedQuery:(NSString*)query {
    return [self getValuesForNonEncryptedQuery:query withLimit:0];
}

- (NSArray*)getValuesForNonEncryptedQuery:(NSString*)query withLimit:(int)limit {
    NSArray* vals = [self.kvDatabase getValsForQuery:query withLimit:limit];
    NSMutableArray* results = [NSMutableArray array];
    for (NSString* val in vals) {
        if (val && [val length]>0){
            NSString* valDec = [KeevaEncryptor decryptToString:val withKey:self.key];
            if (valDec && [valDec length]>0){
                [results addObject:valDec];
            }
        }
    }
    
    return results;
}

- (NSString*)getOneForNonEncryptedQuery:(NSString*)query {
    NSString* val = [self.kvDatabase getOneForQuery:query];
    if (val && [val length]>0){
        NSString* valDec = [KeevaEncryptor decryptToString:val withKey:self.key];
        if (valDec && [valDec length]>0){
            return valDec;
        }
    }
    
    return NULL;
}

- (int)getCountForNonEncryptedQuery:(NSString*)query {
    return [self.kvDatabase getCountForQuery:query];
}

- (NSString*)getValueForNonEncryptedKey:(NSString*)key {
    NSString* val = [self.kvDatabase getValForKey:key];
    if (val && [val length]>0){
        return [KeevaEncryptor decryptToString:val withKey:self.key];
    }
    
    return NULL;
}

- (NSArray *)getEncryptedKeys {
    NSArray *encryptedKeys = [self.kvDatabase getAllKeys];
    NSMutableArray *resultKeys = [NSMutableArray array];
    
    for (NSString *encryptedKey in encryptedKeys) {
        @try {
            NSString *unencryptedKey = [KeevaEncryptor decryptToString:encryptedKey withKey:self.key];
            if (unencryptedKey) {
                [resultKeys addObject:unencryptedKey];
            }
        }
        @catch (NSException *exception) {
            LogDebug(TAG, ([NSString stringWithFormat:@"Exception while decrypting all keys: %@", exception.description]));
        }
    }
    
    return resultKeys;
}

- (void)setValue:(NSString*)val forNonEncryptedKey:(NSString*)key {
    [self.kvDatabase setVal:[KeevaEncryptor encryptString:val withKey:self.key] forKey:key];
}

- (void)deleteValueForNonEncryptedKey:(NSString*)key {
    [self.kvDatabase deleteKeyValWithKey:key];
}

- (void)purge {
    [self.kvDatabase purgeDatabase];
}

static NSString* TAG = @"KEEVA Keeva";

@end
