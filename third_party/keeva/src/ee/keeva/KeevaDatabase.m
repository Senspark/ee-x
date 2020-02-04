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

#import "KeevaDatabase.h"
#import "KeevaConfig.h"
#import "KeevaUtils.h"

// Key Value Table
#define KEYVAL_TABLE_NAME   @"kv_store"
#define KEYVAL_COLUMN_KEY   @"key"
#define KEYVAL_COLUMN_VAL   @"val"


@interface KeevaDatabase()

@property(nonatomic) NSString *dbName;

@end

@implementation KeevaDatabase

- (void)createDBWithPath:(const char *)dbpath {
    if (sqlite3_open(dbpath, &database) == SQLITE_OK)
    {
        char *errMsg;
        
        NSString* createStmt = [NSString stringWithFormat:@"CREATE TABLE IF NOT EXISTS %@ (%@ TEXT PRIMARY KEY, %@ TEXT)", KEYVAL_TABLE_NAME, KEYVAL_COLUMN_KEY, KEYVAL_COLUMN_VAL];
        if (sqlite3_exec(database, [createStmt UTF8String], NULL, NULL, &errMsg) != SQLITE_OK)
        {
            LogDebug(TAG, @"Failed to create key-value table");
        }

        sqlite3_close(database);
        
    } else {
        LogError(TAG, @"Failed to open/create database (createDBWithPath)");
    }
}

- (id)initWithName:(NSString*)dbName {
    @synchronized(self) {
        if (self = [super init]) {
            self.dbName = dbName;
            NSError *error;
            NSString* oldDatabasebPath = [[self applicationDocumentsDirectory] stringByAppendingPathComponent:self.dbName];
            NSString* databasebPath = [[KeevaUtils applicationDirectory] stringByAppendingPathComponent:self.dbName];
            NSFileManager *filemgr = [NSFileManager defaultManager];
            if ([filemgr fileExistsAtPath: oldDatabasebPath] == YES) {
                [[NSFileManager defaultManager] copyItemAtPath:oldDatabasebPath toPath:databasebPath error:&error];
                if (error) {
                    LogError(TAG, @"There was a problem while trying to copy old database.");
                } else {
                    [filemgr removeItemAtPath:oldDatabasebPath error:nil];
                }
            }
            
            if ([filemgr fileExistsAtPath: databasebPath] == NO) {
                [self createDBWithPath:[databasebPath UTF8String]];
                
                NSURL* url = [NSURL fileURLWithPath:databasebPath];
                [KeevaUtils addSkipBackupAttributeToItemAtURL:url];
            }
        }
        return self;
    }
}

- (void)deleteKeyValWithKey:(NSString *)key{
    @synchronized(self) {
        NSString* databasebPath = [[KeevaUtils applicationDirectory] stringByAppendingPathComponent:self.dbName];
        if (sqlite3_open([databasebPath UTF8String], &database) == SQLITE_OK)
        {
            NSString* deleteStmt = [NSString stringWithFormat:@"DELETE FROM %@ WHERE %@=?",
                                    KEYVAL_TABLE_NAME, KEYVAL_COLUMN_KEY];
            sqlite3_stmt *statement;
            if (sqlite3_prepare_v2(database, [deleteStmt UTF8String], -1, &statement, NULL) != SQLITE_OK){
                LogError(TAG, ([NSString stringWithFormat:@"Deleting key:'%@' failed: %s.", key, sqlite3_errmsg(database)]));
            }
            else{
                sqlite3_bind_text(statement, 1, [key UTF8String], -1, SQLITE_TRANSIENT);
                
                if(SQLITE_DONE != sqlite3_step(statement)){
                    NSAssert1(0, @"Error while deleting. '%s'", sqlite3_errmsg(database));
                    sqlite3_reset(statement);
                }
            }
            
            // Finalize and close database.
            sqlite3_finalize(statement);
            sqlite3_close(database);
        }
        else{
            LogError(TAG, @"Failed to open/create database");
        }
    }
}

- (void)purgeDatabase {
    @synchronized(self) {
        NSString* databasebPath = [[KeevaUtils applicationDirectory] stringByAppendingPathComponent:self.dbName];
        if (sqlite3_open([databasebPath UTF8String], &database) == SQLITE_OK)
        {
            NSString* deleteStmt = [NSString stringWithFormat:@"DELETE FROM %@", KEYVAL_TABLE_NAME];
            sqlite3_stmt *statement;
            if (sqlite3_prepare_v2(database, [deleteStmt UTF8String], -1, &statement, NULL) != SQLITE_OK){
                LogError(TAG, ([NSString stringWithFormat:@"Deleting databse failed: %s.", sqlite3_errmsg(database)]));
            }
            else {
                if(SQLITE_DONE != sqlite3_step(statement)){
                    NSAssert1(0, @"Error while purging database. '%s'", sqlite3_errmsg(database));
                    sqlite3_reset(statement);
                }
            }
            
            // Finalize and close database.
            sqlite3_finalize(statement);
            sqlite3_close(database);
        }
        else{
            LogError(TAG, @"Failed to open/create database");
        }
    }
}

- (NSDictionary*)getKeysValsForQuery:(NSString*)query {
    @synchronized(self) {
        NSMutableDictionary *results = [NSMutableDictionary dictionary];
        NSString* databasebPath = [[KeevaUtils applicationDirectory] stringByAppendingPathComponent:self.dbName];
        if (sqlite3_open([databasebPath UTF8String], &database) == SQLITE_OK)
        {
            sqlite3_stmt *statement = nil;
            query = [query stringByReplacingOccurrencesOfString:@"*" withString:@"%"];
            const char *sql = [[NSString stringWithFormat:@"SELECT %@,%@ FROM %@ WHERE %@ LIKE '%@'", KEYVAL_COLUMN_VAL, KEYVAL_COLUMN_KEY, KEYVAL_TABLE_NAME, KEYVAL_COLUMN_KEY, query] UTF8String];
            if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) != SQLITE_OK) {
                LogError(TAG, ([NSString stringWithFormat:@"Error while fetching %@ LIKE '%@' : %s", KEYVAL_COLUMN_KEY, query, sqlite3_errmsg(database)]));
            } else {
                while (sqlite3_step(statement) == SQLITE_ROW) {
                    int colType = sqlite3_column_type(statement, 0);
                    if (colType == SQLITE_TEXT) {
                        const unsigned char *col = sqlite3_column_text(statement, 0);
                        const unsigned char *colkey = sqlite3_column_text(statement, 1);
                        [results setObject:[NSString stringWithFormat:@"%s", col] forKey:[NSString stringWithFormat:@"%s", colkey]];
                    } else {
                        LogError(TAG, @"ERROR: UNKNOWN COLUMN DATATYPE");
                    }
                }
                
                // Finalize
                sqlite3_finalize(statement);
            }
            
            // Close database
            sqlite3_close(database);
        }
        else{
            LogError(TAG, @"Failed to open/create database");
        }
        return results;
    }
}

- (NSArray*)getValsForQuery:(NSString*)query {
    return [self getValsForQuery:query withLimit:0];
}

- (NSArray*)getValsForQuery:(NSString*)query withLimit:(int)limit {
    @synchronized(self) {
        NSMutableArray *results = [NSMutableArray array];
        NSString* databasebPath = [[KeevaUtils applicationDirectory] stringByAppendingPathComponent:self.dbName];
        if (sqlite3_open([databasebPath UTF8String], &database) == SQLITE_OK)
        {
            sqlite3_stmt *statement = nil;
            query = [query stringByReplacingOccurrencesOfString:@"*" withString:@"%"];
            NSString *limitStr = limit > 0? [NSString stringWithFormat:@" LIMIT %d", limit] : @"";
            const char *sql = [[NSString stringWithFormat:@"SELECT %@ FROM %@ WHERE %@ LIKE '%@'%@",
                                KEYVAL_COLUMN_VAL,
                                KEYVAL_TABLE_NAME,
                                KEYVAL_COLUMN_KEY,
                                query,
                                limitStr] UTF8String];
            if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) != SQLITE_OK) {
                LogError(TAG, ([NSString stringWithFormat:@"Error while fetching %@ LIKE '%@'%@ : %s", KEYVAL_COLUMN_KEY, query, limitStr, sqlite3_errmsg(database)]));
            } else {
                while (sqlite3_step(statement) == SQLITE_ROW) {
                    int colType = sqlite3_column_type(statement, 0);
                    if (colType == SQLITE_TEXT) {
                        const unsigned char *col = sqlite3_column_text(statement, 0);
                        [results addObject:[NSString stringWithFormat:@"%s", col]];
                    } else {
                        LogError(TAG, @"ERROR: UNKNOWN COLUMN DATATYPE");
                    }
                }
                
                // Finalize
                sqlite3_finalize(statement);
            }
            
            // Close database
            sqlite3_close(database);
        }
        else{
            LogError(TAG, @"Failed to open/create database");
        }
        return results;
    }
}

- (NSString*)getOneForQuery:(NSString*)query {
    @synchronized(self) {
        NSString *result = NULL;
        NSString* databasebPath = [[KeevaUtils applicationDirectory] stringByAppendingPathComponent:self.dbName];
        if (sqlite3_open([databasebPath UTF8String], &database) == SQLITE_OK)
        {
            sqlite3_stmt *statement = nil;
            query = [query stringByReplacingOccurrencesOfString:@"*" withString:@"%"];
            const char *sql = [[NSString stringWithFormat:@"SELECT %@ FROM %@ WHERE %@ LIKE '%@' LIMIT 1", KEYVAL_COLUMN_VAL, KEYVAL_TABLE_NAME, KEYVAL_COLUMN_KEY, query] UTF8String];
            if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) != SQLITE_OK) {
                LogError(TAG, ([NSString stringWithFormat:@"(getOneForQuery) Error while fetching %@ LIKE '%@' : %s", KEYVAL_COLUMN_KEY, query, sqlite3_errmsg(database)]));
            } else {
                while (sqlite3_step(statement) == SQLITE_ROW) {
                    int colType = sqlite3_column_type(statement, 0);
                    if (colType == SQLITE_TEXT) {
                        const unsigned char *col = sqlite3_column_text(statement, 0);
                        result = [NSString stringWithFormat:@"%s", col];
                    } else {
                        LogError(TAG, @"ERROR: UNKNOWN COLUMN DATATYPE");
                    }
                }
                
                // Finalize
                sqlite3_finalize(statement);
            }
            
            // Close database
            sqlite3_close(database);
        }
        else{
            LogError(TAG, @"Failed to open/create database");
        }
        return result;
    }
}

- (int)getCountForQuery:(NSString*)query {
    @synchronized(self) {
        int count = 0;
        NSString* databasebPath = [[KeevaUtils applicationDirectory] stringByAppendingPathComponent:self.dbName];
        if (sqlite3_open([databasebPath UTF8String], &database) == SQLITE_OK)
        {
            sqlite3_stmt *statement = nil;
            query = [query stringByReplacingOccurrencesOfString:@"*" withString:@"%"];
            const char *sql = [[NSString stringWithFormat:@"SELECT COUNT(%@) FROM %@ WHERE %@ LIKE '%@'", KEYVAL_COLUMN_VAL, KEYVAL_TABLE_NAME, KEYVAL_COLUMN_KEY, query] UTF8String];
            if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) != SQLITE_OK) {
                LogError(TAG, ([NSString stringWithFormat:@"(getOneForQuery) Error while fetching %@ LIKE '%@' : %s", KEYVAL_COLUMN_KEY, query, sqlite3_errmsg(database)]));
            } else {
                while (sqlite3_step(statement) == SQLITE_ROW) {
                    count = sqlite3_column_int(statement, 0);
                }
                
                // Finalize
                sqlite3_finalize(statement);
            }
            
            // Close database
            sqlite3_close(database);
        }
        else{
            LogError(TAG, @"Failed to open/create database");
        }
        return count;
    }
}

- (NSArray *)getAllKeys {
    @synchronized(self) {
        NSMutableArray *results = [NSMutableArray array];
        NSString* databasebPath = [[KeevaUtils applicationDirectory] stringByAppendingPathComponent:self.dbName];
        if (sqlite3_open([databasebPath UTF8String], &database) == SQLITE_OK)
        {
            sqlite3_stmt *statement = nil;
            const char *sql = [[NSString stringWithFormat:@"SELECT %@ FROM %@", KEYVAL_COLUMN_KEY, KEYVAL_TABLE_NAME] UTF8String];
            if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) != SQLITE_OK) {
                LogError(TAG, ([NSString stringWithFormat:@"(getAllKeys) Error while fetching all keys : %s", sqlite3_errmsg(database)]));
            } else {
                while (sqlite3_step(statement) == SQLITE_ROW) {
                    int colType = sqlite3_column_type(statement, 0);
                    if (colType == SQLITE_TEXT) {
                        const unsigned char *col = sqlite3_column_text(statement, 0);
                        [results addObject:[NSString stringWithFormat:@"%s", col]];
                    } else {
                        LogError(TAG, @"ERROR: UNKNOWN COLUMN DATATYPE");
                    }
                }
                
                // Finalize
                sqlite3_finalize(statement);
            }
            
            // Close database
            sqlite3_close(database);
        }
        else{
            LogError(TAG, @"Failed to open/create database");
        }
        return results;
    }
}


- (NSString*)getValForKey:(NSString *)key{
    @synchronized(self) {
        NSString *result = nil;
        NSString* databasebPath = [[KeevaUtils applicationDirectory] stringByAppendingPathComponent:self.dbName];
        if (sqlite3_open([databasebPath UTF8String], &database) == SQLITE_OK)
        {
            sqlite3_stmt *statement = nil;
            const char *sql = [[NSString stringWithFormat:@"SELECT %@ FROM %@ WHERE %@='%@'", KEYVAL_COLUMN_VAL, KEYVAL_TABLE_NAME, KEYVAL_COLUMN_KEY, key] UTF8String];
            if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) != SQLITE_OK) {
                LogError(TAG, ([NSString stringWithFormat:@"Error while fetching %@=%@ : %s", KEYVAL_COLUMN_KEY, key, sqlite3_errmsg(database)]));
            } else {
                while (sqlite3_step(statement) == SQLITE_ROW) {
//                    for (int i=0; i<sqlite3_column_count(statement); i++) {
//                        NSString* colName = [NSString stringWithUTF8String:sqlite3_column_name(statement, i)];
                    
//                        if ([colName isEqualToString:KEYVAL_COLUMN_VAL]) {
                            int colType = sqlite3_column_type(statement, 0);
                            if (colType == SQLITE_TEXT) {
                                const unsigned char *col = sqlite3_column_text(statement, 0);
                                result = [NSString stringWithFormat:@"%s", col];
                            } else {
                                LogError(TAG, @"ERROR: UNKNOWN COLUMN DATATYPE");
                            }
//                        }
//                    }
                }
                
                // Finalize
                sqlite3_finalize(statement);
            }
            
            // Close database
            sqlite3_close(database);
        }
        else{
            LogError(TAG, @"Failed to open/create database");
        }
        return result;
    }
}


- (void)setVal:(NSString *)val forKey:(NSString *)key{
    @synchronized(self) {
        NSString* databasebPath = [[KeevaUtils applicationDirectory] stringByAppendingPathComponent:self.dbName];
        if (sqlite3_open([databasebPath UTF8String], &database) == SQLITE_OK)
        {
            
            NSString* updateStmt = [NSString stringWithFormat:@"UPDATE %@ SET %@=? WHERE %@=?",
                                    KEYVAL_TABLE_NAME, KEYVAL_COLUMN_VAL, KEYVAL_COLUMN_KEY];
            sqlite3_stmt *statement;
            if (sqlite3_prepare_v2(database, [updateStmt UTF8String], -1, &statement, NULL) != SQLITE_OK){
                LogError(TAG, ([NSString stringWithFormat:@"Updating key:'%@' with val:'%@' failed: %s.", key, val, sqlite3_errmsg(database)]));
            }
            else{
                sqlite3_bind_text(statement, 1, [val UTF8String], -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(statement, 2, [key UTF8String], -1, SQLITE_TRANSIENT);
                
                if(SQLITE_DONE != sqlite3_step(statement)){
                    NSAssert1(0, @"Error while updating. '%s'", sqlite3_errmsg(database));
                    sqlite3_reset(statement);
                }
                else {
                    int rowsaffected = sqlite3_changes(database);
                    
                    if (rowsaffected == 0){
                        // Finalize before reuse
                        LogDebug(TAG, @"Finalizing previous statement before reusing it");
                        sqlite3_finalize(statement);
                        
                        LogDebug(TAG, @"Can't update item b/c it doesn't exist. Trying to add a new one.");
                        NSString* addStmt = [NSString stringWithFormat:@"INSERT INTO %@ (%@, %@) VALUES('%@', '%@')",
                                             KEYVAL_TABLE_NAME, KEYVAL_COLUMN_KEY, KEYVAL_COLUMN_VAL, key, val];
                        if (sqlite3_prepare_v2(database, [addStmt UTF8String], -1, &statement, NULL) != SQLITE_OK){
                            LogError(TAG, ([NSString stringWithFormat:@"Adding new item failed: %s. \"George is getting upset!\"", sqlite3_errmsg(database)]));
                        }
                        
                        if(SQLITE_DONE != sqlite3_step(statement)){
                            NSAssert1(0, @"Error while adding item. '%s'", sqlite3_errmsg(database));
                            sqlite3_reset(statement);
                        }
                    }
                }
            }
            
            // Finalize and close database.
            sqlite3_finalize(statement);
            sqlite3_close(database);
        }
        else{
            LogError(TAG, @"Failed to open/create database");
        }
    }
}

#pragma mark - Application's Documents directory

// Returns the URL to the application's Documents directory.
- (NSString *) applicationDocumentsDirectory
{
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *basePath = ([paths count] > 0) ? paths[0] : nil;
        return basePath;
}

static NSString* TAG = @"KEEVA KeevaDatabase";


@end
