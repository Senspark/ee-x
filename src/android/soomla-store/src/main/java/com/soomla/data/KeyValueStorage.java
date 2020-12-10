/*
 * Copyright (C) 2012-2014 Soomla Inc.
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

package com.soomla.data;

import com.soomla.Soomla;
import com.soomla.SoomlaApp;
import com.soomla.SoomlaConfig;
import com.soomla.SoomlaUtils;
import com.soomla.keeva.Keeva;
import com.soomla.keeva.KeevaConfig;

import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.List;

/**
 * This class provides basic storage operations for a simple key-value store.
 */
public class KeyValueStorage {

    /**
     * Retrieves the value for the given key
     *
     * @param key is the key in the key-val pair
     * @return the value for the given key
     */
    public static String getValue(String key) {
        return getDefaultStorage().get(key);
    }

    /**
     * Sets key-val pair in the database according to given key and val.
     *
     * @param key key to set in pair
     * @param val value to set in pair
     */
    public static void setNonEncryptedKeyValue(String key, String val) {
        getDefaultStorage().putForNonEncryptedKey(key, val);
    }

    /**
     * Deletes key-val pair that has the given key.
     *
     * @param key the key to indicate which pair to delete
     */
    public static void deleteNonEncryptedKeyValue(String key) {
        getDefaultStorage().removeForNonEncryptedKey(key);
    }

    /**
     * Retrieves the value of the key-val pair with the given key.
     *
     * @param key key according to which val will be retrieved
     * @return value of key-val pair
     */
    public static String getNonEncryptedKeyValue(String key) {
        return getDefaultStorage().getForNonEncryptedKey(key);
    }

    /**
     * Retrieves key-val pairs according to given query.
     *
     * @param query query that determines what key-val pairs will be returned
     * @return hashmap of key-val pairs
     */
    public static HashMap<String, String> getNonEncryptedQueryValues(String query) {
        return getNonEncryptedQueryValues(query, 0);
    }

    /**
     * Retrieves key-val pairs according to given query, limiting amount of results returned.
     *
     * @param query query that determines what key-val pairs will be returned
     * @param limit max amount of key-val pairs returned
     * @return hashmap of key-val pairs
     */
    public static HashMap<String, String> getNonEncryptedQueryValues(String query, int limit) {
        return getDefaultStorage().getForNonEncryptedQuery(query, limit);
    }

    /**
     * Retrieves one key-val according to given query.
     *
     * @param query query that determines what key-val will be returned
     * @return string of key-val returned
     */
    public static String getOneForNonEncryptedQuery(String query) {
        return getDefaultStorage().oneForNonEncryptedQuery(query);
    }

    /**
     * Retrieves the number key-vals according to given query.
     *
     * @param query query that determines what number of key-vals
     * @return number of key-vals according the the given query
     */
    public static int getCountForNonEncryptedQuery(String query) {
        return getDefaultStorage().countForNonEncryptedQuery(query);
    }

    /**
     * Gets all keys in the storage with no encryption
     *
     * @return a List of unencrypted keys
     */
    public static List<String> getEncryptedKeys() {
        return getDefaultStorage().getOnlyEncryptedKeys();
    }

    /**
     * Sets the given value to the given key.
     *
     * @param key is the key in the key-val pair.
     * @param val is the val in the key-val pair.
     */
    public static void setValue(String key, String val) {
        getDefaultStorage().put(key, val);
    }

    /**
     * Deletes a key-val pair with the given key.
     *
     * @param key is the key in the key-val pair.
     */
    public static void deleteKeyValue(String key) {
        getDefaultStorage().remove(key);
    }

    /**
     * Purges the entire storage
     *
     * NOTE: Use this method with care, it will erase all user data in storage
     * This method is mainly used for testing.
     */
    public static void purge() {
        getDefaultStorage().purgeStorage();
    }

    /**
     * Returns the default KeyValueStorage
     *
     * @return the default KeyValueStorage
     */
    public static synchronized KeyValueStorage getDefaultStorage() {
        if (sSoomlaKeyValueStorage == null) {
            sSoomlaKeyValueStorage = new KeyValueStorage(SOOMLA_DATABASE_NAME, Soomla.SECRET);
        }

        return sSoomlaKeyValueStorage;
    }

    public KeyValueStorage(String storageName, String secret) {
        KeevaConfig.logDebug = SoomlaConfig.logDebug;
        try {
            Field obfuscationSaltField = KeevaConfig.class.getDeclaredField("obfuscationSalt");
            obfuscationSaltField.setAccessible(true);
            obfuscationSaltField.set(null, SoomlaConfig.obfuscationSalt);
            Field dbDeleteField = KeevaConfig.class.getDeclaredField("DB_DELETE");
            dbDeleteField.setAccessible(true);
            dbDeleteField.set(null, SoomlaConfig.DB_DELETE);
        } catch (Exception e) {
            SoomlaUtils.LogError(TAG, "Error setting SOOMLA's config to Keeva " + e.getLocalizedMessage());
        }
        mKeeva = new Keeva(SoomlaApp.getAppContext(), storageName, secret);
    }

    public void purgeStorage() {
        mKeeva.purgeStorage();
    }

    public void remove(String key) {
        mKeeva.remove(key);
    }

    public void put(String key, String val) {
        mKeeva.put(key, val);
    }

    public String get(String key) {
        return mKeeva.get(key);
    }

    public List<String> getOnlyEncryptedKeys() {
        return mKeeva.getOnlyEncryptedKeys();
    }

    public int countForNonEncryptedQuery(String query) {
        return mKeeva.countForNonEncryptedQuery(query);
    }

    public String oneForNonEncryptedQuery(String query) {
        return mKeeva.oneForNonEncryptedQuery(query);
    }

    public HashMap<String, String> getForNonEncryptedQuery(String query) {
        return mKeeva.getForNonEncryptedQuery(query);
    }

    public HashMap<String, String> getForNonEncryptedQuery(String query, int limit) {
        return mKeeva.getForNonEncryptedQuery(query, limit);
    }

    public String getForNonEncryptedKey(String key) {
        return mKeeva.getForNonEncryptedKey(key);
    }

    public void removeForNonEncryptedKey(String key) {
        mKeeva.removeForNonEncryptedKey(key);
    }

    public void putForNonEncryptedKey(String key, String val) {
        mKeeva.putForNonEncryptedKey(key, val);
    }

    /**
     * Private Members
     **/

    private static final String TAG = "SOOMLA KeyValueStorage"; //used for Log Messages
    public static final String SOOMLA_DATABASE_NAME = "store.kv.db";

    private Keeva mKeeva;

    private static KeyValueStorage sSoomlaKeyValueStorage;
}
