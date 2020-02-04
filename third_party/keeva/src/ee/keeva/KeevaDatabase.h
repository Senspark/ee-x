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
#import <sqlite3.h>

/**
 The `KeevaDatabase` provides basic key-value store above SQLite.
 */
@interface KeevaDatabase : NSObject{
    @private
    sqlite3 *database;
}

- (id)initWithName:(NSString*)dbName;

/**
 Sets the given `value` to the given `key`.
 
 @param val The value of the key-val pair.
 @param key The key of the key-val pair.
 */
- (void)setVal:(NSString *)val forKey:(NSString *)key;

/**
 Retrieves the value for the given `key`.
 
 @param key The key of the key-val pair.
 @return The value for the key-val pair.
 */
- (NSString*)getValForKey:(NSString *)key;

/**
 Retrieves from DB the key-val pairs that answer the given `query`.
 
 @param query What to fetch from the DB.
 @return key-val The key-val pairs that answer the given query.
 */
- (NSDictionary*)getKeysValsForQuery:(NSString*)query;

/**
 Retrieves from the DB the values of the key-val pairs that answer the given
 `query`.
 
 @param query The query that indicates what to fetch from the DB.
 @return The values of the key-val pairs fetched.
 */
- (NSArray*)getValsForQuery:(NSString*)query;

/**
 Retrieves from the DB the values of the key-val pairs that answer the given
 `query`.
 
 @param query The query that indicates what to fetch from the DB.
 @param limit the max amount of fetched entries.
 @return The values of the key-val pairs fetched.
 */
- (NSArray*)getValsForQuery:(NSString*)query withLimit:(int)limit;

/**
 Retrieves from the DB ONE value of the key-val pairs that answer the given
 `query`.
 
 @param query The query that indicates what to fetch from the DB.
 @return ONE of the key-val pairs fetched (the first one).
 */
- (NSString*)getOneForQuery:(NSString*)query;

/**
 Counts the number of key-val pairs that answer the given `query`.
 
 @param query The query that indicates what to count in the DB.
 @return the number of key-val pairs matching the query.
 */
- (int)getCountForQuery:(NSString*)query;

/**
 Gets all the keys in the DB
 
 @return a list of `NSString`s containing all the keys in the DB.
 */
- (NSArray *)getAllKeys;

/**
 Deletes from the DB the key-val pair with the given `key`.
 
 @param key The key whose key-val pair is to be deleted.
 */
- (void)deleteKeyValWithKey:(NSString *)key;

/**
 Purges the entire DB
 
 NOTE: Use with caution, since this will delete all the user's local information
 This message is used mainly for while in testing.
 */
- (void)purgeDatabase;

@end
