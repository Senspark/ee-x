/*
 * Copyright (C) 2012 Soomla Inc.
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
package com.soomla.store.billing.google;

import android.content.Context;
import android.content.SharedPreferences;
import android.text.TextUtils;

import com.android.billingclient.api.BillingClient.BillingResponseCode;
import com.android.billingclient.api.Purchase;
import com.android.billingclient.api.SkuDetails;
import com.ee.IStoreBridge;
import com.ee.PluginManagerKt;
import com.ee.internal.StoreException;
import com.soomla.SoomlaApp;
import com.soomla.SoomlaConfig;
import com.soomla.SoomlaUtils;
import com.soomla.data.KeyValueStorage;
import com.soomla.store.SoomlaStore;
import com.soomla.store.billing.IIabService;
import com.soomla.store.billing.IabCallbacks;
import com.soomla.store.billing.IabInventory;
import com.soomla.store.billing.IabPurchase;
import com.soomla.store.billing.IabSkuDetails;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;

import io.reactivex.rxjava3.android.schedulers.AndroidSchedulers;
import io.reactivex.rxjava3.core.Completable;
import io.reactivex.rxjava3.core.Scheduler;
import io.reactivex.rxjava3.schedulers.Schedulers;

/**
 * This is the Google Play plugin implementation of IIabService.
 * <p>
 * see parent for more docs.
 */
public class GooglePlayIabService implements IIabService {
    private IStoreBridge _store;
    private final Scheduler _scheduler;

    public static final String VERSION = "2.2.1";

    public GooglePlayIabService() {
        _scheduler = Schedulers.io();
        configVerifyPurchases(null);    // we reset it every run
    }

    private IStoreBridge getStore() {
        if (_store == null) {
            _store = (IStoreBridge) PluginManagerKt.ee_getStorePlugin();
        }
        return _store;
    }

    @Override
    public void initializeBillingService(IabCallbacks.IabInitListener listener) {
        getStore().connectRx()
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe(
                client -> listener.success(true),
                exception -> listener.fail(exception.getMessage())
            );
    }

    @Override
    public void restorePurchasesAsync(IabCallbacks.OnRestorePurchasesListener listener) {
        getStore().getPurchasesRx(Consts.ITEM_TYPE_INAPP)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe(
                purchaseList -> {
                    IabInventory inventory = new IabInventory();
                    SharedPreferences prefs =
                        SoomlaApp.getAppContext().getSharedPreferences(SoomlaConfig.PREFS_NAME, Context.MODE_PRIVATE);
                    String publicKey = prefs.getString(GooglePlayIabService.PUBLICKEY_KEY, "");
                    for (Purchase purchase : purchaseList) {
                        String purchaseData = purchase.getOriginalJson();
                        String signature = purchase.getSignature();
                        if (Security.verifyPurchase(publicKey, purchaseData, signature)) {
                            IabPurchase iabPurchase = new IabPurchase(Consts.ITEM_TYPE_INAPP, purchaseData, signature);
                            inventory.addPurchase(iabPurchase);
                        }
                    }
                    List<IabPurchase> iabPurchaseList = inventory.getAllPurchases();
                    if (isVerifyPurchasesEnabled()) {
                        listener.verificationStarted(iabPurchaseList);
                        verifyPurchases(iabPurchaseList)
                            .observeOn(AndroidSchedulers.mainThread())
                            .subscribe(
                                () -> listener.success(iabPurchaseList),
                                exception -> listener.fail(exception.getMessage()));
                    } else {
                        listener.success(iabPurchaseList);
                    }
                },
                exception -> listener.fail(exception.getMessage()));
    }

    @Override
    public void fetchSkusDetailsAsync(List<String> skus, IabCallbacks.OnFetchSkusDetailsListener listener) {
        getStore().getSkuDetailsRx(Consts.ITEM_TYPE_INAPP, skus)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe(
                detailsList -> {
                    IabInventory inventory = new IabInventory();
                    for (SkuDetails details : detailsList) {
                        IabSkuDetails iabDetails = new IabSkuDetails(Consts.ITEM_TYPE_INAPP, details.getOriginalJson());
                        inventory.addSkuDetails(iabDetails);
                    }
                    List<IabSkuDetails> iabDetailsList = new ArrayList<>();
                    for (String sku : skus) {
                        IabSkuDetails iabDetails = inventory.getSkuDetails(sku);
                        if (iabDetails != null) {
                            iabDetailsList.add(iabDetails);
                        }
                    }
                    listener.success(iabDetailsList);
                },
                exception -> listener.fail(exception.getMessage()));
    }

    @Override
    public void acknowledgeAsync(IabPurchase purchase, IabCallbacks.OnAcknowledgeListener listener) {
        getStore().acknowledgeRx(purchase.getToken())
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe(() -> listener.success(purchase),
                exception -> listener.fail(exception.getMessage()));

    }

    @Override
    public void consumeAsync(IabPurchase purchase, IabCallbacks.OnConsumeListener listener) {
        getStore().consumeRx(purchase.getToken())
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe(
                () -> listener.success(purchase),
                exception -> listener.fail(exception.getMessage()));
    }

    /**
     * Sets the public key for Google Play IAB Service.
     * This function MUST be called once when the application loads and after SoomlaStore
     * initializes.
     *
     * @param publicKey the public key from the developer console.
     */
    public void setPublicKey(String publicKey) {
        SharedPreferences prefs = SoomlaApp.getAppContext().
            getSharedPreferences(SoomlaConfig.PREFS_NAME, Context.MODE_PRIVATE);
        SharedPreferences.Editor edit = prefs.edit();

        if (publicKey != null && publicKey.length() != 0) {
            edit.putString(PUBLICKEY_KEY, publicKey);
        } else if (prefs.getString(PUBLICKEY_KEY, "").length() == 0) {
            String err = "publicKey is null or empty. Can't initialize store!!";
            SoomlaUtils.LogError(TAG, err);
        }
        edit.commit();
    }

    @Override
    public void configVerifyPurchases(Map<String, Object> config) {
        KeyValueStorage.deleteKeyValue(VERIFY_PURCHASES_KEY);
        KeyValueStorage.deleteKeyValue(VERIFY_CLIENT_ID_KEY);
        KeyValueStorage.deleteKeyValue(VERIFY_CLIENT_SECRET_KEY);
        KeyValueStorage.deleteKeyValue(VERIFY_REFRESH_TOKEN_KEY);
        KeyValueStorage.deleteKeyValue(VERIFY_IAP_URL);
        if (config != null) {
            try {
                checkStringConfigItem(config, "clientId");
                checkStringConfigItem(config, "clientSecret");
                checkStringConfigItem(config, "refreshToken");
            } catch (IllegalArgumentException e) {
                SoomlaUtils.LogError(TAG, e.getMessage());
                return;
            }

            Boolean verifyOnServerFailure = (Boolean) config.get("verifyOnServerFailure");
            if (verifyOnServerFailure == null) {
                verifyOnServerFailure = false;
            }

            String verifyServer = (String) config.get("verifyServer");
            if (verifyServer == null) {
                verifyServer = "";
            }

            KeyValueStorage.setValue(VERIFY_CLIENT_ID_KEY, (String) config.get("clientId"));
            KeyValueStorage.setValue(VERIFY_CLIENT_SECRET_KEY, (String) config.get("clientSecret"));
            KeyValueStorage.setValue(VERIFY_REFRESH_TOKEN_KEY, (String) config.get("refreshToken"));
            KeyValueStorage.setValue(VERIFY_ON_SERVER_FAILURE, verifyOnServerFailure.toString());

            KeyValueStorage.setValue(VERIFY_PURCHASES_KEY, "yes");
            KeyValueStorage.setValue(VERIFY_IAP_URL, verifyServer);
        }
    }

    @Override
    public boolean shouldVerifyPurchases() {
        return isVerifyPurchasesEnabled();
    }

    public void setAccessToken(String token) {
        KeyValueStorage.setValue(VERIFY_ACCESS_TOKEN_KEY, token);
    }

    public String getAccessToken() {
        return KeyValueStorage.getValue(VERIFY_ACCESS_TOKEN_KEY);
    }

    /**
     * see parent
     */
    @Override
    public void launchPurchaseFlow(String itemType, String sku,
                                   IabCallbacks.OnPurchaseListener listener) {
        SharedPreferences prefs = SoomlaApp.getAppContext().
            getSharedPreferences(SoomlaConfig.PREFS_NAME, Context.MODE_PRIVATE);
        String publicKey = prefs.getString(PUBLICKEY_KEY, "");
        if (publicKey.length() == 0 || publicKey.equals("[YOUR PUBLIC KEY FROM THE MARKET]")) {
            SoomlaUtils.LogError(TAG, "You didn't provide a public key! You can't make purchases. the key: " + publicKey);
            throw new IllegalStateException();
        }

        getStore().purchaseRx(sku)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe(
                purchase -> {
                    String purchaseData = purchase.getOriginalJson();
                    String signature = purchase.getSignature();
                    IabPurchase iabPurchase = new IabPurchase(Consts.ITEM_TYPE_INAPP, purchaseData, signature);
                    if (!Security.verifyPurchase(publicKey, purchaseData, signature)) {
                        listener.fail("Local verification failed");
                        return;
                    }
                    if (isVerifyPurchasesEnabled()) {
                        List<IabPurchase> iabPurchaseList = Arrays.asList(iabPurchase);
                        listener.verificationStarted(iabPurchaseList);
                        verifyPurchases(iabPurchaseList)
                            .observeOn(AndroidSchedulers.mainThread())
                            .subscribe(
                                () -> listener.success(iabPurchase),
                                exception -> listener.fail(exception.getMessage())
                            );
                    } else {
                        listener.success(iabPurchase);
                    }
                },
                exception -> {
                    IabPurchase iabPurchase = new IabPurchase(itemType, sku, null, null, 0);
                    if (exception instanceof StoreException) {
                        StoreException storeException = (StoreException) exception;
                        if (storeException.getResponseCode() == BillingResponseCode.USER_CANCELED) {
                            listener.cancelled(iabPurchase);
                            return;
                        }
                        if (storeException.getResponseCode() == BillingResponseCode.ITEM_ALREADY_OWNED) {
                            listener.alreadyOwned(iabPurchase);
                            return;
                        }
                    }
                    listener.fail(exception.getMessage());
                }
            );
    }

    /*====================   Private Utility Methods   ====================*/

    private static boolean isVerifyPurchasesEnabled() {
        return !TextUtils.isEmpty(KeyValueStorage.getValue(VERIFY_PURCHASES_KEY));
    }

    /**
     * Async method - safe to run on ui thread.
     * Verifies purchases using the soomla server.
     */
    private Completable verifyPurchases(List<IabPurchase> purchases) {
        return Completable.create(emitter -> {
            for (IabPurchase purchase : purchases) {
                SoomlaGpVerification sv = new SoomlaGpVerification(purchase,
                    KeyValueStorage.getValue(VERIFY_CLIENT_ID_KEY),
                    KeyValueStorage.getValue(VERIFY_CLIENT_SECRET_KEY),
                    KeyValueStorage.getValue(VERIFY_REFRESH_TOKEN_KEY),
                    Boolean.parseBoolean(KeyValueStorage.getValue(VERIFY_ON_SERVER_FAILURE)));
                sv.verifyPurchase();
            }
            emitter.onComplete();
        }).subscribeOn(_scheduler);
    }

    private void checkStringConfigItem(Map<String, Object> config, String key) {
        Object strToCheck = config.get(key);
        if (!(strToCheck instanceof String) || TextUtils.isEmpty((String) strToCheck)) {
            throw new IllegalArgumentException("Please, provide value for " + key);
        }
    }

    public static GooglePlayIabService getInstance() {
        return (GooglePlayIabService) SoomlaStore.getInstance().getInAppBillingService();
    }

    /* Private Members */
    private static final String TAG = "SOOMLA GooglePlayIabService";

    public static final String PUBLICKEY_KEY = "PO#SU#SO#GU";

    public static final String VERIFY_PURCHASES_KEY = "soomla.verification.verifyPurchases";
    public static final String VERIFY_ON_SERVER_FAILURE = "soomla.verification.verifyOnServerFailure";
    public static final String VERIFY_REFRESH_TOKEN_KEY = "soomla.verification.refreshToken";
    public static final String VERIFY_CLIENT_ID_KEY = "soomla.verification.clientId";
    public static final String VERIFY_CLIENT_SECRET_KEY = "soomla.verification.clientSecret";
    public static final String VERIFY_ACCESS_TOKEN_KEY = "soomla.verification.accessToken";
    public static final String VERIFY_IAP_URL = "soomla.verification.verifyServer";

    /**
     * When set to true, this removes the need to verify purchases when there's no signature.
     * This is useful while you are in development and testing stages of your game.
     * <p>
     * WARNING: Do NOT publish your app with this set to true!!!
     */
    public static boolean AllowAndroidTestPurchases = false;
}
