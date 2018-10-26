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
import com.soomla.data.RewardStorage;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

/**
 * A specific type of <code>Reward</code> that holds of list of other rewards
 * in a certain sequence.  The rewards are given in ascending order.  For example,
 * in a Karate game the user can progress between belts and can be rewarded a
 * sequence of: blue belt, yellow belt, green belt, brown belt, black belt
 *
 * Created by refaelos on 13/05/14.
 */
public class SequenceReward extends Reward {

    /**
     * Constructor
     *
     * @param rewardId see parent
     * @param name see parent
     * @param rewards the list of rewards in the sequence
     *                this must not be null and contain at least 1 item
     */
    public SequenceReward(String rewardId, String name, List<Reward> rewards) {
        super(rewardId, name);

        if (rewards == null || rewards.isEmpty()) {
            final String error = "this reward doesn't make sense without items";
            SoomlaUtils.LogError(TAG, error);
        }

        mRewards = rewards;
    }

    /**
     * Constructor.
     * Generates an instance of <code>SequenceReward</code> from the given <code>JSONObject</code>.
     *
     * @param jsonObject A JSONObject representation of the wanted <code>SequenceReward</code>.
     * @throws JSONException
     */
    public SequenceReward(JSONObject jsonObject) throws JSONException {
        super(jsonObject);

        mRewards = new ArrayList<Reward>();
        JSONArray rewardsArr = jsonObject.optJSONArray(com.soomla.data.JSONConsts.SOOM_REWARDS);
        if (rewardsArr == null) {
            SoomlaUtils.LogWarning(TAG, "reward has no meaning without children");
            rewardsArr = new JSONArray();
        }

        for(int i=0; i<rewardsArr.length(); i++) {
            JSONObject rewardJSON = rewardsArr.getJSONObject(i);
            Reward reward = fromJSONObject(rewardJSON);
            if (reward != null) {
                mRewards.add(reward);
            }
        }
    }

    /**
     * Converts the current <code>SequenceReward</code> to a JSONObject.
     *
     * @return A <code>JSONObject</code> representation of the current <code>SequenceReward</code>.
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
     * Retrieves the last reward that was given from the sequence.
     *
     * @return the last given reward
     */
    public Reward getLastGivenReward() {
        int idx = RewardStorage.getLastSeqIdxGiven(this.getID());
        if (idx < 0) {
            return null;
        }
        return mRewards.get(idx);
    }

    /**
     * Checks if there are more rewards in the sequence that can be given.
     *
     * @return <code>true</code> if there are more rewards, <code>false</code> otherwise
     */
    public boolean hasMoreToGive() {
        return RewardStorage.getLastSeqIdxGiven(this.getID()) < mRewards.size() ;
    }

    /**
     * Forces which reward to give next in the sequence.
     * NOTE: doesn't not give the skipped rewards
     *
     * @param reward the reward to give next
     * @return <code>true</code> if the reward was set successfully as next
     * in the sequence, <code>false</code> otherwise.
     */
    public boolean forceNextRewardToGive(Reward reward) {
        for (int i = 0; i < mRewards.size(); i++) {
            if (mRewards.get(i).getID().equals(reward.getID())) {
                RewardStorage.setLastSeqIdxGiven(this.getID(), i - 1);
                return true;
            }
        }
        return false;
    }

    /**
     * Gives the next reward in the sequence.
     *
     * @return <code>true</code> if the next reward was given,
     * <code>false</code> if the sequence has been completed and there
     * are no more rewards to be given.
     */
    @Override
    protected boolean giveInner() {
        int idx = RewardStorage.getLastSeqIdxGiven(this.getID());
        if (idx >= mRewards.size()) {
            return false; // all rewards in the sequence were given
        }
        RewardStorage.setLastSeqIdxGiven(this.getID(), ++idx);
        return true;
    }

    @Override
    protected boolean takeInner() {
        int idx = RewardStorage.getLastSeqIdxGiven(this.getID());
        if (idx <= 0) {
            return false; // all rewards in the sequence were taken
        }
        RewardStorage.setLastSeqIdxGiven(this.getID(), --idx);
        return true;
    }

    /** Setters and Getters **/

    public List<Reward> getRewards() {
        return mRewards;
    }


    /** Private Members **/

    private static final String TAG = "SOOMLA SequenceReward";

    private List<Reward> mRewards;
}
