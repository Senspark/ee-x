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

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 * A specific type of <code>Reward</code> that holds of list of other
 * rewards. When this reward is given, it randomly chooses a reward from
 * the list of rewards it internally holds.
 * Currently, the pool of rewards stays constant, so each reward is available
 * on each draw.
 * (A future version may add subtracting a given reward from the pool)
 *
 * For example: a user can earn a mystery box
 * reward (<code>RandomReward</code>, which in fact grants the user a random reward between a
 * "Mayor" badge (<code>BadgeReward</code>) and a speed boost (<code>VirtualItemReward</code>)
 *
 * Created by refaelos on 13/05/14.
 */
public class RandomReward extends Reward {

    /**
     * Constructor
     *
     * @param rewardId see parent
     * @param name see parent
     * @param rewards a list of rewards from which to choose the reward randomly
     *                this must not be null and contain at least 1 item
     */
    public RandomReward(String rewardId, String name, List<Reward> rewards) {
        super(rewardId, name);

        if (rewards == null || rewards.isEmpty()) {
            final String error = "this reward doesn't make sense without items";
            SoomlaUtils.LogError(TAG, error);
        }

        mRewards = rewards;
    }

    /**
     * Constructor.
     * Generates an instance of <code>RandomReward</code> from the given <code>JSONObject</code>.
     *
     * @param jsonObject A JSONObject representation of the wanted <code>RandomReward</code>.
     * @throws JSONException
     */
    public RandomReward(JSONObject jsonObject) throws JSONException {
        super(jsonObject);

        mRewards = new ArrayList<Reward>();
        JSONArray rewardsArr = jsonObject.optJSONArray(com.soomla.data.JSONConsts.SOOM_REWARDS);
        if (rewardsArr == null) {
            SoomlaUtils.LogWarning(TAG, "reward has no meaning without children");
            rewardsArr = new JSONArray();
        }

        // Iterate over all rewards in the JSON array and for each one create
        // an instance according to the reward type
        for (int i = 0; i < rewardsArr.length(); i++) {
            JSONObject rewardJSON = rewardsArr.getJSONObject(i);
            Reward reward = Reward.fromJSONObject(rewardJSON);
            if (reward != null) {
                mRewards.add(reward);
            }
        }
    }

    /**
     * Converts the current <code>RandomReward</code> to a JSONObject.
     *
     * @return A <code>JSONObject</code> representation of the current <code>RandomReward</code>.
     */
    @Override
    public JSONObject toJSONObject(){
        JSONObject jsonObject = super.toJSONObject();
        try {
            JSONArray rewardsArr = new JSONArray();
            for (Reward reward : mRewards) {
                rewardsArr.put(reward.toJSONObject());
            }
            jsonObject.put(com.soomla.data.JSONConsts.SOOM_REWARDS, rewardsArr);
        } catch (JSONException e) {
            SoomlaUtils.LogError(TAG, "An error occurred while generating JSON object.");
        }

        return jsonObject;
    }

    /**
     * Gives a random reward from the list of rewards.
     *
     * @return <code>true</code>
     */
    @Override
    protected boolean giveInner() {
        List<Reward> canBeGivenRewards = new ArrayList<Reward>();
        for (Reward reward : this.mRewards) {
            if (reward.canGive()) {
                canBeGivenRewards.add(reward);
            }
        }

        if (canBeGivenRewards.isEmpty()) {
            SoomlaUtils.LogDebug(TAG, "No more rewards to give in this Random Reward: " + this.mID);
            return false;
        }

        Random rand = new Random();
        int n = rand.nextInt(canBeGivenRewards.size());
        final Reward randomReward = canBeGivenRewards.get(n);
        randomReward.give();
        mLastGivenReward = randomReward;

        return true;
    }

    @Override
    protected boolean takeInner() {
        // for now is able to take only last given
        if(mLastGivenReward == null) {
            return false;
        }

        final boolean taken = mLastGivenReward.take();
        mLastGivenReward = null;

        return taken;
    }

    /** Setters and Getters **/

    public List<Reward> getRewards() {
        return mRewards;
    }


    /** Private Members **/

    private static final String TAG = "SOOMLA RandomReward";

    private List<Reward> mRewards;
    private Reward mLastGivenReward;
}
