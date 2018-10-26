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

import android.content.Context;
import android.text.TextUtils;

import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * This class provides basic storage operations for a simple key-value store.
 */
public class Keeva {

    /**
     * Constructor TODO javadoc
     *
     * @param context
     * @param storageName
     * @param secret
     */
    public Keeva(Context context, String storageName, String secret) {
        if (secret == null) {
            throw new InvalidParameterException("You must initialize Keeva with a secret. storageName: " + mStorageName);
        }

        mAppContext = context.getApplicationContext();
        mStorageName = storageName;
        mSecret = secret;
        mKvDatabase = new KeevaDatabase(mAppContext, storageName);
        mObfuscator = new AESObfuscator(KeevaConfig.obfuscationSalt,
                mAppContext.getPackageName(),
                KeevaUtils.deviceId(mAppContext),
                mSecret);
    }

    /**
     * Purges the entire storage
     *
     * NOTE: Use this method with care, it will erase all user data in storage
     * This method is mainly used for testing.
     */
    public void purgeStorage() {
        KeevaUtils.LogDebug(TAG, "purging database" + (mStorageName != null ? " in storage: " + mStorageName : ""));

        mKvDatabase.purgeDatabaseEntries(mAppContext);
    }

    /**
     * Deletes a key-val pair with the given key.
     *
     * @param key is the key in the key-val pair.
     */
    public void remove(String key) {
        KeevaUtils.LogDebug(TAG, "deleting " + key + (mStorageName != null ? " from storage: " + mStorageName : ""));

        key = mObfuscator.obfuscateString(key);

        mKvDatabase.deleteKeyVal(key);
    }

    /**
     * Sets the given value to the given key.
     *
     * @param key is the key in the key-val pair.
     * @param val is the val in the key-val pair.
     */
    public void put(String key, String val) {

        KeevaUtils.LogDebug(TAG, "setting " + val + " for key: " + key + (mStorageName != null ? " in storage: " + mStorageName : ""));

        key = mObfuscator.obfuscateString(key);
        val = mObfuscator.obfuscateString(val);

        mKvDatabase.setKeyVal(key, val);
    }

    /**
     * Retrieves the value for the given key
     *
     * @param key is the key in the key-val pair
     * @return the value for the given key
     */
    public String get(String key) {
        KeevaUtils.LogDebug(TAG, "trying to fetch a value for key: " + key + (mStorageName != null ? " from storage: " + mStorageName : ""));
        key = mObfuscator.obfuscateString(key);

        String val = mKvDatabase.getKeyVal(key);

        if (val != null && !TextUtils.isEmpty(val)) {
            try {
                val = mObfuscator.unobfuscateToString(val);
            } catch (AESObfuscator.ValidationException e) {
                KeevaUtils.LogError(TAG, e.getMessage());
                val = "";
            }

            KeevaUtils.LogDebug(TAG, "the fetched value is " + val);
        }
        return val;
    }

    /**
     * Gets all keys in the storage with no encryption
     *
     * @return a List of unencrypted keys
     */
    public List<String> getOnlyEncryptedKeys() {

        KeevaUtils.LogDebug(TAG, "trying to fetch all keys" + (mStorageName != null ? " from storage: " + mStorageName : ""));

        List<String> encryptedKeys = mKvDatabase.getAllKeys();
        List<String> resultKeys = new ArrayList<String>();

        for (String encryptedKey : encryptedKeys) {
            try {
                String unencryptedKey = mObfuscator.unobfuscateToString(encryptedKey);
                resultKeys.add(unencryptedKey);
            } catch (AESObfuscator.ValidationException e) {
                KeevaUtils.LogDebug(TAG, e.getMessage());
            } catch (RuntimeException e) {
                KeevaUtils.LogError(TAG, e.getMessage());
            }
        }

        return resultKeys;
    }

    /**
     * Retrieves the number key-vals according to given query.
     *
     * @param query query that determines what number of key-vals
     * @return number of key-vals according the the given query
     */
    public int countForNonEncryptedQuery(String query) {
        KeevaUtils.LogDebug(TAG, "trying to fetch count for query: " + query + (mStorageName != null ? " from storage: " + mStorageName : ""));

        return mKvDatabase.getQueryCount(query);
    }

    /**
     * Retrieves one key-val according to given query.
     *
     * @param query query that determines what key-val will be returned
     * @return string of key-val returned
     */
    public String oneForNonEncryptedQuery(String query) {

        KeevaUtils.LogDebug(TAG, "trying to fetch one for query: " + query + (mStorageName != null ? " from storage: " + mStorageName : ""));

        String val = mKvDatabase.getQueryOne(query);
        if (val != null && !TextUtils.isEmpty(val)) {
            try {
                val = mObfuscator.unobfuscateToString(val);
                return val;
            } catch (AESObfuscator.ValidationException e) {
                KeevaUtils.LogError(TAG, e.getMessage());
            }
        }

        return null;
    }

    /**
     * Retrieves key-val pairs according to given query.
     *
     * @param query query that determines what key-val pairs will be returned
     * @return hashmap of key-val pairs
     */
    public HashMap<String, String> getForNonEncryptedQuery(String query) {
        return getForNonEncryptedQuery(query, 0);
    }

    /**
     * Retrieves key-val pairs according to given query, limiting amount of results returned.
     *
     * @param query query that determines what key-val pairs will be returned
     * @param limit max amount of key-val pairs returned
     * @return hashmap of key-val pairs
     */
    public HashMap<String, String> getForNonEncryptedQuery(String query, int limit) {
        KeevaUtils.LogDebug(TAG, "trying to fetch values for query: " + query +
                (limit > 0 ? " with limit: " + limit : "") +
                (mStorageName != null ? " from storage: " + mStorageName : ""));

        HashMap<String, String> vals = mKvDatabase.getQueryVals(query, limit);
        HashMap<String, String> results = new HashMap<String, String>();
        for (String key : vals.keySet()) {
            String val = vals.get(key);
            if (val != null && !TextUtils.isEmpty(val)) {
                try {
                    val = mObfuscator.unobfuscateToString(val);
                    results.put(key, val);
                } catch (AESObfuscator.ValidationException e) {
                    KeevaUtils.LogError(TAG, e.getMessage());
                }
            }
        }

        KeevaUtils.LogDebug(TAG, "fetched " + results.size() + " results");

        return results;
    }

    /**
     * Retrieves the value of the key-val pair with the given key.
     *
     * @param key key according to which val will be retrieved
     * @return value of key-val pair
     */
    public String getForNonEncryptedKey(String key) {

        KeevaUtils.LogDebug(TAG, "trying to fetch a value for key: " + key + (mStorageName != null ? " from storage: " + mStorageName : ""));

        String val = mKvDatabase.getKeyVal(key);

        if (val != null && !TextUtils.isEmpty(val)) {
            try {
                val = mObfuscator.unobfuscateToString(val);
            } catch (AESObfuscator.ValidationException e) {
                KeevaUtils.LogError(TAG, e.getMessage());
                val = "";
            }

            KeevaUtils.LogDebug(TAG, "the fetched value is " + val);
        }
        return val;
    }

    /**
     * Deletes key-val pair that has the given key.
     *
     * @param key the key to indicate which pair to delete
     */
    public void removeForNonEncryptedKey(String key) {
        KeevaUtils.LogDebug(TAG, "deleting " + key + (mStorageName != null ? " from storage: " + mStorageName : ""));

        mKvDatabase.deleteKeyVal(key);
    }

    /**
     * Sets key-val pair in the database according to given key and val.
     *
     * @param key key to set in pair
     * @param val value to set in pair
     */
    public void putForNonEncryptedKey(String key, String val) {

        KeevaUtils.LogDebug(TAG, "setting " + val + " for key: " + key + (mStorageName != null ? " in storage: " + mStorageName : ""));

        val = mObfuscator.obfuscateString(val);
        mKvDatabase.setKeyVal(key, val);
    }


    /**
     * Private Members
     **/

    private static final String TAG = "KEEVA Keeva"; //used for Log Messages

    private Context mAppContext;
    private AESObfuscator mObfuscator;
    private KeevaDatabase mKvDatabase;
    private String mStorageName;
    private String mSecret;

}
