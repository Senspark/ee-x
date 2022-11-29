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
import android.content.SharedPreferences;
import android.provider.Settings;
import android.text.TextUtils;
import android.util.Log;

import java.util.UUID;

/**
 * This class provides Log functions that output debug, warning, or error messages.
 */
public class KeevaUtils {

    /**
     * Creates Log Debug message according to given tag and message.
     *
     * @param tag the name of the class whose instance called this function
     * @param message debug message to output to log
     */
    public static void LogDebug(String tag, String message) {
        if (KeevaConfig.logDebug) {
            Log.d(tag, message);
        }
    }

    /**
     * Creates Log Warning message according to given tag and message.
     *
     * @param tag the name of the class whose instance called this function
     * @param message warning message to output to log
     */
    public static void LogWarning(String tag, String message) {
        Log.w(tag, message);
    }

    /**
     * Creates Log Error message according to given tag and message.
     *
     * @param tag the name of the class whose instance called this function
     * @param message error message to output to log
     */
    public static void LogError(String tag, String message) {
        Log.e(tag, message);
    }

    /**
     * Retrieves Android device Id.
     *
     * @return androidId which is the id of the device being used
     */
    public static String deviceId(Context context) {
        String androidId = Settings.Secure.getString(context.getApplicationContext().getContentResolver(),
                Settings.Secure.ANDROID_ID);
        if (androidId == null) {
            // This is a fallback in case the device id cannot be retrieved on the device
            // (happened on some devices !)
            KeevaUtils.LogError("KEEVA ObscuredSharedPreferences",
                    "Couldn't fetch ANDROID_ID. Using generated id.");
            androidId = generateKeevaId(context);
        }

        return androidId;
    }

	private static String generateKeevaId(Context context) {
        SharedPreferences sharedPrefs = context.getSharedPreferences("store.kv.db", Context.MODE_PRIVATE);
		String generatedId = sharedPrefs.getString(DB_KEY_SOOMLA_GENERATED_ID, null);
		if (TextUtils.isEmpty(generatedId)) {
			String id = UUID.randomUUID().toString();
            sharedPrefs.edit().putString(DB_KEY_SOOMLA_GENERATED_ID, id).commit();
		}
		return generatedId;
	}


    /** Private Members **/

	private static final String DB_KEY_SOOMLA_GENERATED_ID = "soomla.generatedid";
}
