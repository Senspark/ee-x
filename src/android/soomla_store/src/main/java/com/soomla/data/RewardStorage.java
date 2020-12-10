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

package com.soomla.data;

import android.text.TextUtils;

import com.soomla.BusProvider;
import com.soomla.SoomlaConfig;
import com.soomla.SoomlaUtils;
import com.soomla.events.RewardGivenEvent;
import com.soomla.events.RewardTakenEvent;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;
import java.util.List;

/**
 * A utility class for persisting and querying the state of rewards.
 * Use this class to check if a certain reward was given, or to
 * set its state.
 * This class uses the <code>KeyValueStorage</code> internally for storage.
 *
 * Created by refaelos on 13/05/14.
 */
public class RewardStorage {

    private static final String TAG = "SOOMLA RewardStorage";

    private static final String DB_KEY_REWARDS = SoomlaConfig.DB_KEY_PREFIX + "rewards.";

    private static String keyRewards(String rewardId, String postfix) {
        return DB_KEY_REWARDS + rewardId + "." + postfix;
    }

    private static String keyRewardTimesGiven(String rewardId) {
        return keyRewards(rewardId, "timesGiven");
    }

    private static String keyRewardLastGiven(String rewardId) {
        return keyRewards(rewardId, "lastGiven");
    }

    private static String keyRewardIdxSeqGiven(String rewardId) {
        return keyRewards(rewardId, "seq.idx");
    }


    /** Badges **/

    /**
     * Sets the reward status of the given reward\
     *
     * @param rewardId the reward to set status
     * @param give <code>true</code>
     */
    public static void setRewardStatus(String rewardId, boolean give) {
        setRewardStatus(rewardId, give, true);
    }

    public static void setRewardStatus(String rewardId, boolean give, boolean notify) {
        setTimesGiven(rewardId, give, notify);
    }

    /**
     * Checks whether the given reward was given.
     *
     * @param rewardId the reward to check
     * @return <code>true</code> if the reward was already given,
     * <code>false</code> otherwise
     */
    public static boolean isRewardGiven(String rewardId) {
        return getTimesGiven(rewardId) > 0;
    }


    /** Sequence Reward **/

    /**
     * Retrieves the index of the last reward given in a sequence of rewards.
     *
     * @param rewardId the SequenceReward to check
     * @return the index of the reward in the sequence
     */
    public static int getLastSeqIdxGiven(String rewardId) {
        String key = keyRewardIdxSeqGiven(rewardId);

        String val = KeyValueStorage.getValue(key);

        if (val == null) {
            return -1;
        }
        return Integer.parseInt(val);
    }

    /**
     * Sets the index of the last reward given in a sequence of rewards.
     *
     * @param rewardId the SequenceReward who's index is to be set
     * @param idx the index to set
     */
    public static void setLastSeqIdxGiven(String rewardId, int idx) {
        String key = keyRewardIdxSeqGiven(rewardId);

        KeyValueStorage.setValue(key, String.valueOf(idx));
    }

    public static int getTimesGiven(String rewardId) {
        String key = keyRewardTimesGiven(rewardId);
        String val = KeyValueStorage.getValue(key);
        if (TextUtils.isEmpty(val)) {
            return 0;
        }
        return Integer.parseInt(val);
    }

    public static Date getLastGivenTime(String rewardId) {
        long timeMillis = getLastGivenTimeMillis(rewardId);
        if (timeMillis == 0) {
            return null;
        }
        Date toReturn = new Date();
        toReturn.setTime(timeMillis);
        return toReturn;
    }

    public static long getLastGivenTimeMillis(String rewardId) {
        String key = keyRewardLastGiven(rewardId);
        String val = KeyValueStorage.getValue(key);
        if (TextUtils.isEmpty(val)) {
            return 0;
        }
        return Long.parseLong(val);

    }

    public static void setLastGivenTimeMillis(String rewardId, long lastGiven) {
        String key = keyRewardLastGiven(rewardId);
        KeyValueStorage.setValue(key, String.valueOf(lastGiven));
    }

    private static void setTimesGiven(String rewardId, boolean up, boolean notify) {
        int total = getTimesGiven(rewardId) + (up ? 1 : -1);
        resetTimesGiven(rewardId, total);

        if (up) {
            setLastGivenTimeMillis(rewardId, new Date().getTime());
        }

        if (notify) {
            if (up) {
                BusProvider.getInstance().post(new RewardGivenEvent(rewardId));
            } else {
                BusProvider.getInstance().post(new RewardTakenEvent(rewardId));
            }
        }
    }

    public static void resetTimesGiven(String rewardId, int timesGiven) {
        String key = keyRewardTimesGiven(rewardId);
        KeyValueStorage.setValue(key, String.valueOf(timesGiven));
    }

    public static JSONObject getRewardsState() {
        List<String> rewardIds = getRewardIds();
        JSONObject rewardsStateJSON = new JSONObject();

        for (String rewardId : rewardIds) {
            JSONObject rewardValuesJSON = new JSONObject();
            try {
                int timesGiven = RewardStorage.getTimesGiven(rewardId);
                rewardValuesJSON.put("timesGiven", timesGiven);

                long lastGiven = RewardStorage.getLastGivenTimeMillis(rewardId);
                rewardValuesJSON.put("lastGiven", lastGiven);

                //TODO: add LastSeqIdxGiven when sequence reward is fixed

                rewardsStateJSON.put(rewardId, rewardValuesJSON);
            }
            catch (JSONException e) {
                SoomlaUtils.LogDebug(TAG, "Unable to set reward " + rewardId + " state. error: " + e.getLocalizedMessage());
            }
        }

        return rewardsStateJSON;
    }

    public static boolean resetRewardsState(JSONObject state) {
        if (state == null) {
            return false;
        }

        List<String> rewardIds = getRewardIds();

        try {
            Iterator keysIter = state.keys();
            while (keysIter.hasNext()) {
                String rewardId = (String) keysIter.next();
                JSONObject itemValuesJSON = state.getJSONObject(rewardId);

                if (itemValuesJSON.has("timesGiven")) {
                    int timesGiven = itemValuesJSON.getInt("timesGiven");
                    resetTimesGiven(rewardId, timesGiven);
                }

                if (itemValuesJSON.has("lastGiven")) {
                    long lastGiven = itemValuesJSON.getLong("lastGiven");
                    setLastGivenTimeMillis(rewardId, lastGiven);
                }

                rewardIds.remove(rewardId);
            }
        }
        catch (JSONException e) {
            SoomlaUtils.LogError(TAG, "Unable to set state for rewards. error: " + e.getLocalizedMessage());
            return false;
        }

        // When resetting state we should remove all rewards' state which
        // were not in the sync state (so the state is inline with the provided
        // state)
        for (String rewardId : rewardIds) {
            KeyValueStorage.deleteKeyValue(keyRewardTimesGiven(rewardId));
            KeyValueStorage.deleteKeyValue(keyRewardLastGiven(rewardId));
            KeyValueStorage.deleteKeyValue(keyRewardIdxSeqGiven(rewardId));
        }

        return true;
    }

    private static List<String> getRewardIds() {
        List<String> kvKeys = KeyValueStorage.getEncryptedKeys();
        List<String> rewardIds = new ArrayList<String>();
        if (kvKeys == null) {
            return rewardIds;
        }

        for (String key : kvKeys) {
            if (key.startsWith(DB_KEY_REWARDS)) {
                String rewardId = key.replace(DB_KEY_REWARDS, "");
                int dotIndex = rewardId.indexOf('.');
                if (dotIndex != -1) {
                    rewardId = rewardId.substring(0, dotIndex);
                }
                if (!rewardIds.contains(rewardId)) {
                    rewardIds.add(rewardId);
                }
            }
        }

        return rewardIds;
    }
}
