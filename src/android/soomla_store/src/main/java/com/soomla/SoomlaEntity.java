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

import android.text.TextUtils;

import com.soomla.data.JSONConsts;

import org.json.JSONException;
import org.json.JSONObject;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;


/**
 *
 * This is the parent class of all entities in the application.
 * Almost every entity in your virtual economy will be an entity. There
 * are many types of entities - each one of the various types extends
 * <code>SoomlaEntity</code> and adds its own behavior on top of it.
 */
public abstract class SoomlaEntity<T> {

    /**
     * Constructor.
     *
     * @param mName the name of the entity
     * @param mDescription the description of the entity
     * @param mID the ID of the entity
     */
    public SoomlaEntity(String mName, String mDescription, String mID) {
        this.mName = mName;
        this.mDescription = mDescription;
        this.mID = mID.trim();
    }

    /**
     * Constructor.
     * Generates an instance of <code>SoomlaEntity</code> from the given <code>JSONObject</code>.
     *
     * @param jsonObject A JSONObject representation of the wanted <code>SoomlaEntity</code>.
     * @throws org.json.JSONException
     */
    public SoomlaEntity(JSONObject jsonObject) throws JSONException {
        mName = jsonObject.optString(JSONConsts.SOOM_ENTITY_NAME);
        mDescription = jsonObject.optString(JSONConsts.SOOM_ENTITY_DESCRIPTION);
        mID = jsonObject.getString(JSONConsts.SOOM_ENTITY_ID);
    }

    /**
     * Converts the current <code>SoomlaEntity</code> to a JSONObject.
     *
     * @return A <code>JSONObject</code> representation of the current <code>SoomlaEntity</code>.
     */
    public JSONObject toJSONObject(){
        if (TextUtils.isEmpty(this.mID)) {
            SoomlaUtils.LogError(TAG, "This is BAD! We don't have ID in the this SoomlaEntity. Stopping here.");
            return null;
        }

        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put(JSONConsts.SOOM_ENTITY_NAME, mName);
            jsonObject.put(JSONConsts.SOOM_ENTITY_DESCRIPTION, mDescription);
            jsonObject.put(JSONConsts.SOOM_ENTITY_ID, mID);
            jsonObject.put(JSONConsts.SOOM_CLASSNAME, SoomlaUtils.getClassName(this));
        } catch (JSONException e) {
            SoomlaUtils.LogError(TAG, "An error occurred while generating JSON object.");
        }

        return jsonObject;
    }

    /**
     * Checks if the given object is equal to this object, by comparing the given object's
     * id with this <code>SoomlaEntity</code>'s ID
     *
     * @param o the object to compare
     * @return true if the objects are equal, otherwise false
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof SoomlaEntity)) return false;

        SoomlaEntity that = (SoomlaEntity) o;

        return mID.equals(that.mID);
    }

    /**
     * @{inheritDoc}
     */
    @Override
    public int hashCode() {
        return mID != null ? mID.hashCode() : 0;
    }

    public T clone(String newId) {
        JSONObject obj = this.toJSONObject();
        try {
            obj.put(JSONConsts.SOOM_ENTITY_ID, newId);

            final Constructor<? extends SoomlaEntity> jsonCtor = ((Class<? extends SoomlaEntity>)getClass()).getDeclaredConstructor(JSONObject.class);
            return (T) jsonCtor.newInstance(obj);

        } catch (JSONException e) {
            SoomlaUtils.LogError(TAG, "Error when trying to clone SoomlaEntity(" + getClass().getSimpleName() + "): " + e.getLocalizedMessage());
        } catch (InvocationTargetException e) {
            SoomlaUtils.LogError(TAG, "Error when trying to clone SoomlaEntity(" + getClass().getSimpleName() + "): " + e.getLocalizedMessage());
        } catch (NoSuchMethodException e) {
            SoomlaUtils.LogError(TAG, "Error when trying to clone SoomlaEntity(" + getClass().getSimpleName() + "): " + e.getLocalizedMessage());
        } catch (InstantiationException e) {
            SoomlaUtils.LogError(TAG, "Error when trying to clone SoomlaEntity(" + getClass().getSimpleName() + "): " + e.getLocalizedMessage());
        } catch (IllegalAccessException e) {
            SoomlaUtils.LogError(TAG, "Error when trying to clone SoomlaEntity(" + getClass().getSimpleName() + "): " + e.getLocalizedMessage());
        }
        return null;
    }

    /** Setters and Getters **/

    public String getName() {
        return mName;
    }

    public String getDescription() {
        return mDescription;
    }

    public String getID() {
        return mID;
    }

    /** Private Members **/

    private static final String TAG = "SOOMLA SoomlaEntity"; //used for Log messages

    protected String mName;
    protected String mDescription;
    protected String mID;
}
