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

package com.soomla.rewards;

import com.soomla.SoomlaUtils;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * A specific type of <code>Reward</code> that represents a badge
 * with an icon. For example: when the user achieves a top score,
 * the user can earn a "Highest Score" badge reward.
 *
 * Created by refaelos on 13/05/14.
 */
public class BadgeReward extends Reward {

    /**
     * Constructor
     *
     * @param rewardId see parent
     * @param name see parent
     */
    public BadgeReward(String rewardId, String name) {
        super(rewardId, name);
    }

    /**
     * Constructor
     *
     * @param rewardId see parent
     * @param name see parent
     * @param iconUrl the URL of the badge's icon
     */
    public BadgeReward(String rewardId, String name, String iconUrl) {
        super(rewardId, name);
        mIconUrl = iconUrl;
    }

    /**
     * Constructor.
     * Generates an instance of <code>BadgeReward</code> from the given <code>JSONObject</code>.
     *
     * @param jsonObject A JSONObject representation of the wanted <code>BadgeReward</code>.
     * @throws JSONException
     */
    public BadgeReward(JSONObject jsonObject) throws JSONException {
        super(jsonObject);
        try {
            mIconUrl = jsonObject.getString(com.soomla.data.JSONConsts.SOOM_REWARD_ICONURL);
        } catch (JSONException ignored) {
            SoomlaUtils.LogError(TAG, "An error occurred while generating JSON object.");
        }
    }

    /**
     * Converts the current <code>BadgeReward</code> to a JSONObject.
     *
     * @return A <code>JSONObject</code> representation of the current <code>BadgeReward</code>.
     */
    @Override
    public JSONObject toJSONObject(){
        JSONObject jsonObject = super.toJSONObject();
        try {
            jsonObject.put(com.soomla.data.JSONConsts.SOOM_REWARD_ICONURL, mIconUrl);
        } catch (JSONException e) {
            SoomlaUtils.LogError(TAG, "An error occurred while generating JSON object.");
        }

        return jsonObject;
    }

    @Override
    protected boolean giveInner() {

        // nothing to do here... the parent Reward gives in storage
        return true;
    }

    @Override
    protected boolean takeInner() {
        // nothing to do here... the parent Reward takes in storage
        return true;
    }

    /** Setters and Getters **/

    public String getIconUrl() {
        return mIconUrl;
    }

    public void setIconUrl(String iconUrl) {
        mIconUrl = iconUrl;
    }


    /** Private Members **/

    private static final String TAG = "SOOMLA BadgeReward";

    private String mIconUrl;
}
