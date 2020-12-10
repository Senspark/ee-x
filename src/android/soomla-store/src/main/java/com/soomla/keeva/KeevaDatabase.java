/*
 * Copyright (C) 2012-2016 Soomla Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.soomla.keeva;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * The KeevaDatabase provides a basic key-value store above SQLite.
 */
public class KeevaDatabase {

    /**
     * Constructor.
     *
     * @param context global information about the application environment
     * @param dbName name of the db to use
     */
    public KeevaDatabase(Context context, String dbName) {

        if (KeevaConfig.DB_DELETE){
            context.deleteDatabase(dbName);
        }

        mDatabaseHelper = new DatabaseHelper(context, dbName);
        mStoreDB = mDatabaseHelper.getWritableDatabase();
    }

    /**
     * Closes the database.
     */
    public synchronized void close() {
        mDatabaseHelper.close();
    }

    /**
     * Deletes the database completely!
     *
     * @param context global information about the application environment
     * @param dbName name of the db to purge
     */
    public void purgeDatabase(Context context, String dbName) {
        context.deleteDatabase(dbName);
    }

    /**
     * Deletes the database contents
     *
     * @param context global information about the application environment
     */
    public void purgeDatabaseEntries(Context context) {
        mStoreDB.delete(KEYVAL_TABLE_NAME, null, null);
    }

    /**
     * Sets the given value to the given key.
     *
     * @param key the key of the key-val pair
     * @param val the val of the key-val pair
     */
    public synchronized void setKeyVal(String key, String val) {
        ContentValues values = new ContentValues();
        values.put(KEYVAL_COLUMN_VAL, val);

        int affected = mStoreDB.update(KEYVAL_TABLE_NAME, values, KEYVAL_COLUMN_KEY + "='"
                + key + "'", null);
        if (affected == 0){
            values.put(KEYVAL_COLUMN_KEY, key);
            mStoreDB.replace(KEYVAL_TABLE_NAME, null, values);
        }
    }

    /**
     * Retrieves the value for the given key.
     *
     * @param key the key of the key-val pair
     * @return a value for the given key
     */
    public synchronized String getKeyVal(String key) {
        Cursor cursor = null;
        try {
            cursor = mStoreDB.query(KEYVAL_TABLE_NAME, KEYVAL_COLUMNS, KEYVAL_COLUMN_KEY
                            + "='" + key + "'",
                    null, null, null, null);

            if (cursor != null && cursor.moveToNext()) {
                int valColIdx = cursor.getColumnIndexOrThrow(KEYVAL_COLUMN_VAL);
                return  cursor.getString(valColIdx);
            }
        } finally {
            if (cursor != null) {
                cursor.close();
            }
        }

        return null;
    }

    /**
     * Deletes the key-val pair.
     *
     * @param key the key of the key-val pair
     */
    public synchronized void deleteKeyVal(String key) {
        mStoreDB.delete(KEYVAL_TABLE_NAME, KEYVAL_COLUMN_KEY + "=?", new String[] { key });
    }

    public synchronized HashMap<String, String> getQueryVals(String query) {
        return getQueryVals(query, 0);
    }

	public synchronized HashMap<String, String> getQueryVals(String query, int limit) {
        Cursor cursor = null;
        try {
            query = query.replace('*', '%');
            cursor = mStoreDB.query(KEYVAL_TABLE_NAME, KEYVAL_COLUMNS, KEYVAL_COLUMN_KEY
                            + " LIKE '" + query + "'",
                    null, null, null, null, limit <= 0 ? null : Integer.toString(limit));

            HashMap<String, String> ret = new HashMap<String, String>();
            while (cursor != null && cursor.moveToNext()) {
                try {
                    int valColIdx = cursor.getColumnIndexOrThrow(KEYVAL_COLUMN_VAL);
                    int keyColIdx = cursor.getColumnIndexOrThrow(KEYVAL_COLUMN_KEY);
                    ret.put(cursor.getString(keyColIdx), cursor.getString(valColIdx));
                } catch (IllegalArgumentException exx) {
                }
            }
            return ret;
        } finally {
            if(cursor != null) {
                cursor.close();
            }
        }
	}

    public synchronized String getQueryOne(String query) {
        Cursor cursor = null;
        try {
            query = query.replace('*', '%');
            cursor = mStoreDB.query(KEYVAL_TABLE_NAME, KEYVAL_COLUMNS, KEYVAL_COLUMN_KEY
                            + " LIKE '" + query + "'",
                    null, null, null, null, "1");

            if (cursor != null) {
                boolean moved = cursor.moveToFirst();
                if (moved) {
                    int valColIdx = cursor.getColumnIndexOrThrow(KEYVAL_COLUMN_VAL);
                    return cursor.getString(valColIdx);
                }
            }
        } finally {
            if(cursor != null) {
                cursor.close();
            }
        }
//        String ret = null;
//        while (cursor != null && cursor.moveToNext()) {
//            try {
//                int valColIdx = cursor.getColumnIndexOrThrow(KEYVAL_COLUMN_VAL);
//                ret = cursor.getString(valColIdx);
//            } catch (IllegalArgumentException exx) {
//            }
//        }

        return null;

    }

    public synchronized int getQueryCount(String query) {
        Cursor cursor = null;
        try {
            query = query.replace('*', '%');
            cursor = mStoreDB.rawQuery("SELECT COUNT(" + KEYVAL_COLUMN_VAL + ") from " +
                    KEYVAL_TABLE_NAME + " WHERE " + KEYVAL_COLUMN_KEY + " LIKE '" + query + "'", null);
            if (cursor != null) {
                boolean moved = cursor.moveToFirst();
                if (moved) {
                    return cursor.getInt(0);
                }
            }
        } finally {
            if(cursor != null) {
                cursor.close();
            }
        }

        return 0;
    }

    public synchronized List<String> getAllKeys() {
        Cursor cursor = null;
        try {
            cursor = mStoreDB.query(KEYVAL_TABLE_NAME, new String[]{KEYVAL_COLUMN_KEY},
                    null, null, null, null, null);
            List<String> ret = new ArrayList<String>();
            while (cursor != null && cursor.moveToNext()) {
                try {
                    int keyColIdx = cursor.getColumnIndexOrThrow(KEYVAL_COLUMN_KEY);
                    ret.add(cursor.getString(keyColIdx));
                } catch (IllegalArgumentException exx) {
                }
            }
            return ret;
        } finally {
            if(cursor != null) {
                cursor.close();
            }
        }
    }

    /**
     * DataBase Helper class
     */
    private class DatabaseHelper extends SQLiteOpenHelper{

        public DatabaseHelper(Context context, String dbName) {
            super(context, dbName, null, 1);
        }

        @Override
        public void onCreate(SQLiteDatabase sqLiteDatabase) {
            if (!sqLiteDatabase.isReadOnly()){
                sqLiteDatabase.execSQL("PRAGMA foreign_key=ON");
            }

            sqLiteDatabase.execSQL("CREATE TABLE IF NOT EXISTS " + KEYVAL_TABLE_NAME + "(" +
                    KEYVAL_COLUMN_KEY + " TEXT PRIMARY KEY, " +
                    KEYVAL_COLUMN_VAL + " TEXT)");
        }

        @Override
        public void onUpgrade(SQLiteDatabase sqLiteDatabase, int i, int i1) {
            // Nothing to do here ...
        }
    }


    /** General key-value storage */

    private static final String KEYVAL_TABLE_NAME = "kv_store";
    public static final String KEYVAL_COLUMN_KEY = "key";
    public static final String KEYVAL_COLUMN_VAL = "val";
    private static final String[] KEYVAL_COLUMNS = {
            KEYVAL_COLUMN_KEY, KEYVAL_COLUMN_VAL
    };


    /** Private Members **/

    private static final String TAG = "KeevaDatabase"; //used for Log messages

    private SQLiteDatabase mStoreDB;

    private DatabaseHelper mDatabaseHelper;
}
