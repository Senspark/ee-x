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

import com.soomla.Schedule;
import com.soomla.SoomlaEntity;
import com.soomla.SoomlaUtils;
import com.soomla.data.JSONConsts;
import com.soomla.data.RewardStorage;
import com.soomla.util.JSONFactory;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * A reward is an entity which can be earned by the user for meeting certain
 * criteria in game progress.  For example - a user can earn a badge for completing
 * a mission. Dealing with <code>Reward</code>s is very similar to dealing with
 * <code>VirtualItem</code>s: grant a reward by giving it and recall a
 * reward by taking it.
 */
public abstract class Reward extends SoomlaEntity<Reward> {

    /**
     * Constructor
     *
     * @param rewardId the reward's ID (unique id to distinguish between rewards).
     * @param name the reward's name.
     */
    public Reward(String rewardId, String name) {
        super(name, "", rewardId);
        mSchedule = Schedule.AnyTimeOnce();
    }

    /**
     * Constructor.
     * Generates an instance of <code>Reward</code> from the given <code>JSONObject</code>.
     *
     * @param jsonObject A JSONObject representation of the wanted <code>Reward</code>.
     * @throws JSONException
     */
    public Reward(JSONObject jsonObject) throws JSONException {
        super(jsonObject);
        JSONObject scheduleObj = jsonObject.optJSONObject(JSONConsts.SOOM_SCHEDULE);
        if (scheduleObj != null) {
            mSchedule = Schedule.AnyTimeOnce();
        } else {
            mSchedule = null;
        }
    }

    /**
     * Converts the current <code>Reward</code> to a JSONObject.
     *
     * @return A <code>JSONObject</code> representation of the current <code>Reward</code>.
     */
    @Override
    public JSONObject toJSONObject(){
        JSONObject jsonObject = super.toJSONObject();
        try {
            jsonObject.put(JSONConsts.SOOM_SCHEDULE, mSchedule.toJSONObject());
        } catch (JSONException e) {
            SoomlaUtils.LogError(TAG, "An error occurred while generating JSON object.");
        }

        return jsonObject;
    }

    public static Reward fromJSONString(String jsonString) {
        try {
            JSONObject rewardObj = new JSONObject(jsonString);
            return fromJSONObject(rewardObj);
        } catch (JSONException e) {
            return null;
        }
    }

    public static Reward fromJSONObject(JSONObject jsonObject) {
        return sJSONFactory.create(jsonObject, Reward.class.getPackage().getName());
    }

    /**
     * Grants this reward to the user. Use this method in cases where the user
     * has positive progress in game play and is eligible for earning this reward.
     * For example - give a reward when a user completes a mission or a challenge.
     *
     * @return if the reward was actually given
     */
    public boolean give() {
        if (!canGive()) {
            SoomlaUtils.LogDebug(TAG, "(Give) Reward is not approved by Schedule. id: " + mID);
            return false;
        }

        if (giveInner()) {
            RewardStorage.setRewardStatus(this.getID(), true);
            return true;
        }

        return false;
    }

    /**
     * Takes this reward from the user. Use this method in cases where the user
     * should be "punished", or has negative progress in the game
     * indicating that his \ her previously earned reward should be recalled.
     */
    public boolean take() {
        if (!RewardStorage.isRewardGiven(this.getID())) {
            SoomlaUtils.LogDebug(TAG, "Reward not given. id: " + mID);
            return false;
        }

        if (takeInner()) {
            RewardStorage.setRewardStatus(this.getID(), false);
            return true;
        }

        return false;
    }

    /**
     * Checks if this reward can be given
     *
     * @return <code>true</code> if can be given, <code>false</code> otherwise
     */
    public boolean canGive() {
        return mSchedule.approve(RewardStorage.getTimesGiven(this.getID()));
    }

    /**
     * Checks if the user owns this reward.
     *
     * @return <code>true</code> if owned, <code>false</code> otherwise
     */
    public boolean isOwned() {
        return RewardStorage.isRewardGiven(this.getID());
    }

    /**
     * Tests the reward criteria which need to be met in order
     * to <code>give</code> the user this reward
     *
     * @return <code>true</code> if criteria is met, <code>false</code> otherwise
     */
    protected abstract boolean giveInner();

    /**
     * Tests the reward criteria which need to be met in order
     * to <code>take</code> the reward from the user
     * (not always possible?)
     *
     * @return <code>true</code> if criteria is met, <code>false</code> otherwise
     */
    protected abstract boolean takeInner();

    /** Setters and Getters **/

    public Schedule getSchedule() {
        return mSchedule;
    }

    /** Private Members **/

    private static final String TAG = "SOOMLA Reward";

    private static JSONFactory<Reward> sJSONFactory = new JSONFactory<Reward>();

    protected Schedule mSchedule;
}

