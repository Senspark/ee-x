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

package com.soomla.util;

import com.soomla.SoomlaUtils;

import org.json.JSONException;
import org.json.JSONObject;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

/**
 * Utility to create LevelUp objects from JSON
 * (Pretty much one step before using Gson/Jackson)
 */
public class JSONFactory<T> {
    public static final String TAG = "SOOMLA JSONFactory";

    public T create(JSONObject jsonObject, String packageName) {

        if (jsonObject == null) {
            // warn
            return null;
        }

        T t = null;
        try {
//            SoomlaUtils.LogDebug(TAG, jsonObject.toString());
            String className = jsonObject.getString(com.soomla.data.JSONConsts.SOOM_CLASSNAME);
            Class<? extends T> clazz = (Class<? extends T>) Class.forName(packageName + "." + className);
            SoomlaUtils.LogDebug(TAG, "creating with: " + packageName + "." + className);
            if (clazz != null) {
                final Constructor<? extends T> jsonCtor = clazz.getDeclaredConstructor(JSONObject.class);
                t = jsonCtor.newInstance(jsonObject);
            }
            else {
                SoomlaUtils.LogError(TAG, "unknown class name:" + className);
            }

        } catch (JSONException e) {
            SoomlaUtils.LogError(TAG, "fromJSONObject JSONException:" + e.getMessage());
        } catch (InstantiationException e) {
            SoomlaUtils.LogError(TAG, "fromJSONObject InstantiationException:" + e.getMessage());
        } catch (IllegalAccessException e) {
            SoomlaUtils.LogError(TAG, "fromJSONObject IllegalAccessException:" + e.getMessage());
        } catch (NoSuchMethodException e) {
            SoomlaUtils.LogError(TAG, "fromJSONObject no JSONObject constructor found:" + e.getMessage());
        } catch (InvocationTargetException e) {
            SoomlaUtils.LogError(TAG, "fromJSONObject InvocationTargetException:" + e.getMessage());
            SoomlaUtils.LogError(TAG, "fromJSONObject InvocationTargetException[cause]:" + e.getCause());
        } catch (ClassNotFoundException e) {
            SoomlaUtils.LogError(TAG, "fromJSONObject ClassNotFoundException:" + e.getMessage());
        }

        return t;
    }
}
