package com.soomla.cocos2dx.common;

import android.app.Activity;
import android.opengl.GLSurfaceView;

import com.soomla.Soomla;
import com.soomla.SoomlaUtils;
import com.soomla.data.KeyValueStorage;
import com.soomla.data.RewardStorage;
import com.soomla.rewards.BadgeReward;
import com.soomla.rewards.RandomReward;
import com.soomla.rewards.SequenceReward;
import com.soomla.Schedule;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.lang.Class;
import java.lang.reflect.Method;
import java.lang.Exception;
import java.lang.String;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class CoreBridge {

    private static final String KVS_MIGRATED_EMPTY_KEY = "soomla_kvs_migrated_empty_key";

    private static CoreBridge INSTANCE = null;

    public static CoreBridge getInstance() {
        if (INSTANCE == null) {
            synchronized (CoreBridge.class) {
                if (INSTANCE == null) {
                    INSTANCE = new CoreBridge();
                }
            }
        }
        return INSTANCE;
    }

    @SuppressWarnings("FieldCanBeLocal")
    private CoreEventHandlerBridge coreEventHandlerBridge;

    public CoreBridge() {
        coreEventHandlerBridge = new CoreEventHandlerBridge();

        DomainHelper.getInstance().registerTypeWithClassName(CommonConsts.JSON_JSON_TYPE_SCHEDULE, Schedule.class);
        DomainHelper.getInstance().registerTypeWithClassName(CommonConsts.JSON_JSON_TYPE_DATE_TIME_RANGE, Schedule.DateTimeRange.class);

        final DomainFactory domainFactory = DomainFactory.getInstance();

        domainFactory.registerCreator(CommonConsts.JSON_JSON_TYPE_BADGE, new DomainFactory.Creator<BadgeReward>() {
            @Override
            public BadgeReward create(JSONObject jsonObject) {
                try {
                    return new BadgeReward(jsonObject);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
        domainFactory.registerCreator(CommonConsts.JSON_JSON_TYPE_RANDOM, new DomainFactory.Creator<RandomReward>() {
            @Override
            public RandomReward create(JSONObject jsonObject) {
                try {
                    return new RandomReward(jsonObject);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
        domainFactory.registerCreator(CommonConsts.JSON_JSON_TYPE_SEQUENCE, new DomainFactory.Creator<SequenceReward>() {
            @Override
            public SequenceReward create(JSONObject jsonObject) {
                try {
                    return new SequenceReward(jsonObject);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });

        final NdkGlue ndkGlue = NdkGlue.getInstance();

        ndkGlue.registerCallHandler("CCSoomla::initialize", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String soomlaSecret = params.optString("soomlaSecret");
                SoomlaUtils.LogDebug("SOOMLA", "initialize is called from java!");

                Method method = Class.forName("org.cocos2dx.lib.Cocos2dxActivity").getMethod("getContext");
                Activity cocos2dxActivity = (Activity)method.invoke(null);
                Soomla.initialize(cocos2dxActivity, soomlaSecret);

                if (KeyValueStorage.getDefaultStorage().get(KVS_MIGRATED_EMPTY_KEY) == null) {
                    migrateEmptyKeyInKVS();
                    KeyValueStorage.getDefaultStorage().put(KVS_MIGRATED_EMPTY_KEY, "true");
                }

            }
        });
        ndkGlue.registerCallHandler("CCNativeRewardStorage::getTimesGiven", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String rewardId = params.getString("reward");
                int result = RewardStorage.getTimesGiven(rewardId);
                retParams.put("return", result);
            }
        });
        ndkGlue.registerCallHandler("CCNativeRewardStorage::setRewardStatus", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String rewardId = params.getString("reward");
                boolean give =  params.getBoolean("give");
                boolean notify =  params.getBoolean("notify");
                RewardStorage.setRewardStatus(rewardId, give, notify);
            }
        });
        ndkGlue.registerCallHandler("CCNativeRewardStorage::getLastSeqIdxGiven", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String rewardId = params.getString("reward");
                int result = RewardStorage.getLastSeqIdxGiven(rewardId);
                retParams.put("return", result);
            }
        });
        ndkGlue.registerCallHandler("CCNativeRewardStorage::setLastSeqIdxGiven", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String rewardId = params.getString("reward");
                int idx =  params.getInt("idx");
                RewardStorage.setLastSeqIdxGiven(rewardId, idx);
            }
        });

        ndkGlue.registerCallHandler("CCNativeKeyValueStorage::getValue", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String key =  params.getString("key");
                String result = KeyValueStorage.getValue(key);
                retParams.put("return", result);
            }
        });
        ndkGlue.registerCallHandler("CCNativeKeyValueStorage::setValue", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String key =  params.getString("key");
                String val =  params.getString("val");
                KeyValueStorage.setValue(key, val);
            }
        });
        ndkGlue.registerCallHandler("CCNativeKeyValueStorage::deleteKeyValue", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String key =  params.getString("key");
                KeyValueStorage.deleteKeyValue(key);
            }
        });
        ndkGlue.registerCallHandler("CCNativeKeyValueStorage::purge", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                KeyValueStorage.purge();
            }
        });
        ndkGlue.registerCallHandler("CCNativeKeyValueStorage::getEncryptedKeys", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                List<String> result = KeyValueStorage.getEncryptedKeys();
                retParams.put("return", new JSONArray(result));
            }
        });
    }

    private static void migrateEmptyKeyInKVS() {
        KeyValueStorage kvs = new KeyValueStorage("store.kv.db", "");

        // Remove encrypted keys and save them in a Map
        List<String> keys = kvs.getOnlyEncryptedKeys();
        Map<String, String> encryptedStorage = new HashMap<String, String>();
        for (String key : keys) {
            String val = kvs.get(key);
            encryptedStorage.put(key, val);
            kvs.remove(key);
        }

        // Migrate non-encrypted keys
        HashMap<String, String> nonEncrypted = kvs.getForNonEncryptedQuery("*");
        for (String key : nonEncrypted.keySet()) {
            KeyValueStorage.getDefaultStorage().putForNonEncryptedKey(key, nonEncrypted.get(key));
        }

        // Save back encrypted storage
        for (String key : encryptedStorage.keySet()) {
            KeyValueStorage.getDefaultStorage().put(key, encryptedStorage.get(key));
        }

    }

    public void init() {
        final GLSurfaceView glSurfaceView = NdkGlue.getInstance().getGlSurfaceRef().get();
        if (glSurfaceView != null) {
            coreEventHandlerBridge.setGlSurfaceView(glSurfaceView);
        }
    }
}
