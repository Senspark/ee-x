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

package com.soomla.store;

/**
 * This class holds the store's configurations.
 *
 * IMPORTANT: Before releasing your game, set <code>AllowAndroidTestPurchases</code> to false !!!
 */
public class StoreConfig {

    // A friendlyRefunds tells android-store if to let your refunded users keep their VirtualItems
    // after a refund or not. (default: false)
    public static final boolean friendlyRefunds = false;

    /**
     * ---NEVER!--- CHANGE THE VALUE FOR THIS VARIABLE !!!
     * This value defines the version of the metadata located in your database.
     */
    public static final int METADATA_VERSION = 4;

}
