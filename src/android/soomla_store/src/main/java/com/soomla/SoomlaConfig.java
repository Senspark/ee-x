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

package com.soomla;

/**
 * This class holds the store's configurations.
 *
 * IMPORTANT: Change <code>obfuscationSalt</code> below !!!
 * IMPORTANT: Before releasing your game, set <code>DB_DELETE</code> to false !!!
 */
public class SoomlaConfig {

    public static int SOOMLA_VERSION = 1;

    //Set to true if you want to print out debug messages
    public static boolean logDebug = false;

    /**
     * The obfuscated salt: randomly generated numbers.
     * IMPORTANT: it's recommended that you change these numbers for your specific application,
     * BUT change them only once!
     */
    public static final byte[] obfuscationSalt = new byte[] { 64, -54, -113, -47, 98, -52, 87,
            -102, -65, -127, 89, 51, -11, -35, 30, 77, -45, 75, -26, 3 };

    /**
     * If this is true then the database will be deleted whenever the application loads.
     *
     * WARNING: Do NOT release your game with this option set to true!
     * Otherwise, your users will lose all their data every time they load the application.
     *
     * NOTE: this feature can be useful for testing when you want to change stuff in your
     * implementation of <code>IStoreAssets</code> and see what they look like. If you try to
     * change things in <code>IStoreAssets</code> and don't delete the DB then your changes will
     * not be shown.
     */
    public static final boolean DB_DELETE = false;

    public static final String DB_KEY_PREFIX = "soomla.";

    /** Shared Preferences **/
    public static final String PREFS_NAME      = "store.prefs"; //TODO check if needed
    public static final String DB_INITIALIZED  = "db_initialized";

}
