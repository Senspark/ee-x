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

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Process;
import android.text.TextUtils;
import com.soomla.SoomlaApp;
import com.soomla.SoomlaConfig;
import com.soomla.SoomlaUtils;
import com.soomla.data.KeyValueStorage;
import com.soomla.store.SoomlaStore;
import com.soomla.store.billing.IIabService;
import com.soomla.store.billing.IabCallbacks;
import com.soomla.store.billing.IabException;
import com.soomla.store.billing.IabHelper;
import com.soomla.store.billing.IabInventory;
import com.soomla.store.billing.IabPurchase;
import com.soomla.store.billing.IabResult;
import com.soomla.store.billing.IabSkuDetails;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;

/**
 * This is the Google Play plugin implementation of IIabService.
 *
 * see parent for more docs.
 */
public class GooglePlayIabService implements IIabService {

    public static final String VERSION = "1.1.2";

    public GooglePlayIabService() {
        configVerifyPurchases(null);    // we reset it every run
    }

    /**
     * see parent
     */
    @Override
    public void initializeBillingService(final IabCallbacks.IabInitListener iabListener) {

        // Set up helper for the first time, querying and synchronizing inventory
        startIabHelper(new OnIabSetupFinishedListener(iabListener));
    }

    /**
     * see parent
     */
    @Override
    public void startIabServiceInBg(IabCallbacks.IabInitListener iabListener) {
        keepIabServiceOpen = true;
        startIabHelper(new OnIabSetupFinishedListener(iabListener));
    }

    /**
     * see parent
     */
    @Override
    public void stopIabServiceInBg(IabCallbacks.IabInitListener iabListener) {
        keepIabServiceOpen = false;
        stopIabHelper(iabListener);
    }

    /**
     * see parent
     */
    @Override
    public void restorePurchasesAsync(IabCallbacks.OnRestorePurchasesListener restorePurchasesListener) {
        mHelper.restorePurchasesAsync(new RestorePurchasesFinishedListener(restorePurchasesListener));
    }

    /**
     * see parent
     */
    @Override
    public void fetchSkusDetailsAsync(List<String> skus, IabCallbacks.OnFetchSkusDetailsListener fetchSkusDetailsListener) {
        mHelper.fetchSkusDetailsAsync(skus, new FetchSkusDetailsFinishedListener(fetchSkusDetailsListener));
    }

    /**
     * see parent
     */
    @Override
    public boolean isIabServiceInitialized() {
        return mHelper != null;
    }

    /**
     * see parent
     */
    @Override
    public void consume(IabPurchase purchase) throws IabException {
        mHelper.consume(purchase);
    }

    /**
     * see parent
     */
    @Override
    public void consumeAsync(IabPurchase purchase, final IabCallbacks.OnConsumeListener consumeListener) {
        mHelper.consumeAsync(purchase, new GoogleIabHelper.OnConsumeFinishedListener() {
            @Override
            public void onConsumeFinished(IabPurchase purchase, IabResult result) {
                if (result.isSuccess()) {

                    consumeListener.success(purchase);
                } else {

                    consumeListener.fail(result.getMessage());
                }
            }
        });
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

            KeyValueStorage.setValue(VERIFY_CLIENT_ID_KEY, (String) config.get("clientId"));
            KeyValueStorage.setValue(VERIFY_CLIENT_SECRET_KEY, (String) config.get("clientSecret"));
            KeyValueStorage.setValue(VERIFY_REFRESH_TOKEN_KEY, (String) config.get("refreshToken"));
            KeyValueStorage.setValue(VERIFY_ON_SERVER_FAILURE, verifyOnServerFailure.toString());

            KeyValueStorage.setValue(VERIFY_PURCHASES_KEY, "yes");
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
    public void launchPurchaseFlow(String itemType,
                                   String sku,
                                   final IabCallbacks.OnPurchaseListener purchaseListener,
                                   String extraData) {

        SharedPreferences prefs = SoomlaApp.getAppContext().
                getSharedPreferences(SoomlaConfig.PREFS_NAME, Context.MODE_PRIVATE);
        String publicKey = prefs.getString(PUBLICKEY_KEY, "");
        if (publicKey.length() == 0 || publicKey.equals("[YOUR PUBLIC KEY FROM THE MARKET]")) {
            SoomlaUtils.LogError(TAG, "You didn't provide a public key! You can't make purchases. the key: " + publicKey);
            throw new IllegalStateException();
        }


        try {
            final Intent intent = new Intent(SoomlaApp.getAppContext(), IabActivity.class);
            intent.putExtra(SKU, sku);
            intent.putExtra(ITEM_TYPE, itemType);
            intent.putExtra(EXTRA_DATA, extraData);

            mSavedOnPurchaseListener = purchaseListener;
            if (SoomlaApp.getAppContext() instanceof Activity) {
                Activity activity = (Activity) SoomlaApp.getAppContext();
                activity.startActivity(intent);
            } else {
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                SoomlaApp.getAppContext().startActivity(intent);
            }

        } catch(Exception e){
            String msg = "(launchPurchaseFlow) Error purchasing item " + e.getMessage();
            SoomlaUtils.LogError(TAG, msg);
            purchaseListener.fail(msg);
        }

    }



    /*====================   Private Utility Methods   ====================*/

    /**
     * Create a new IAB helper and set it up.
     *
     * @param onIabSetupFinishedListener is a callback that lets users to add their own implementation for when the Iab is started
     */
    private synchronized void startIabHelper(OnIabSetupFinishedListener onIabSetupFinishedListener) {
        if (isIabServiceInitialized())
        {
            SoomlaUtils.LogDebug(TAG, "The helper is started. Just running the post start function.");

            if (onIabSetupFinishedListener != null && onIabSetupFinishedListener.getIabInitListener() != null) {
                onIabSetupFinishedListener.getIabInitListener().success(true);
            }
            return;
        }

        SoomlaUtils.LogDebug(TAG, "Creating IAB helper.");
        mHelper = new GoogleIabHelper();

        SoomlaUtils.LogDebug(TAG, "IAB helper Starting setup.");
        mHelper.startSetup(onIabSetupFinishedListener);
    }

    /**
     * Dispose of the helper to prevent memory leaks
     */
    private synchronized void stopIabHelper(IabCallbacks.IabInitListener iabInitListener) {
        if (keepIabServiceOpen) {
            String msg = "Not stopping Google Service b/c the user run 'startIabServiceInBg'. Keeping it open.";
            if (iabInitListener != null) {
                iabInitListener.fail(msg);
            } else {
                SoomlaUtils.LogDebug(TAG, msg);
            }
            return;
        }

        if (mHelper == null) {
            String msg = "Tried to stop Google Service when it was null.";
            if (iabInitListener != null) {
                iabInitListener.fail(msg);
            } else {
                SoomlaUtils.LogDebug(TAG, msg);
            }
            return;
        }

        if (!mHelper.isAsyncInProgress())
        {
            SoomlaUtils.LogDebug(TAG, "Stopping Google Service");
            mHelper.dispose();
            mHelper = null;
            if (iabInitListener != null) {
                iabInitListener.success(true);
            }
        }
        else
        {
            String msg = "Cannot stop Google Service during async process. Will be stopped when async operation is finished.";
            if (iabInitListener != null) {
                iabInitListener.fail(msg);
            } else {
                SoomlaUtils.LogDebug(TAG, msg);
            }
        }
    }

    private static boolean isVerifyPurchasesEnabled() {
        return !TextUtils.isEmpty(KeyValueStorage.getValue(VERIFY_PURCHASES_KEY));
    }

    /**
     * Async method - safe to run on ui thread.
     * Verifies purchases using the soomla server.
     */
    private void verifyPurchases(final List<IabPurchase> purchases, final VerifyPurchasesFinishedListener listener) {

        new Thread(new Runnable() {
            @Override
            public void run() {
                Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);

                for (IabPurchase purchase : purchases) {

                    SoomlaGpVerification sv = new SoomlaGpVerification(purchase,
                            KeyValueStorage.getValue(VERIFY_CLIENT_ID_KEY),
                            KeyValueStorage.getValue(VERIFY_CLIENT_SECRET_KEY),
                            KeyValueStorage.getValue(VERIFY_REFRESH_TOKEN_KEY),
                            Boolean.parseBoolean(KeyValueStorage.getValue(VERIFY_ON_SERVER_FAILURE)));

                    sv.verifyPurchase();
                }

                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        listener.finished();
                    }
                });

            }
        }).start();
    }

    /**
     * Callback for verify purchases.
     */
    public interface VerifyPurchasesFinishedListener {
        /**
         * Celled to notify that a verify purchases operation completed.
         */
        public void finished();

    }

    /**
     * Handle Restore Purchases processes
     */
    private class RestorePurchasesFinishedListener implements IabHelper.RestorePurchasessFinishedListener {


        private IabCallbacks.OnRestorePurchasesListener mRestorePurchasesListener;

        public RestorePurchasesFinishedListener(IabCallbacks.OnRestorePurchasesListener restorePurchasesListener) {
            this.mRestorePurchasesListener            = restorePurchasesListener;
        }

        @Override
        public void onRestorePurchasessFinished(IabResult result, IabInventory inventory) {
            SoomlaUtils.LogDebug(TAG, "Restore Purchases succeeded");
            if (result.getResponse() == IabResult.BILLING_RESPONSE_RESULT_OK && mRestorePurchasesListener != null) {
                // fetching owned items
                List<String> itemSkus = inventory.getAllOwnedSkus();

                final List<IabPurchase> purchases = new ArrayList<IabPurchase>();
                for (String sku : itemSkus) {
                    IabPurchase purchase = inventory.getPurchase(sku);
                    purchases.add(purchase);
                }

                if (isVerifyPurchasesEnabled()) {
                    mRestorePurchasesListener.verificationStarted(purchases);
                    verifyPurchases(purchases, new VerifyPurchasesFinishedListener() {
                        @Override
                        public void finished() {
                            restorePurchasessFinished(purchases);
                        }
                    });
                } else {
                    restorePurchasessFinished(purchases);
                }

            } else {
                SoomlaUtils.LogError(TAG, "Either mRestorePurchasesListener==null OR Restore purchases error: " + result.getMessage());
                if (this.mRestorePurchasesListener != null) this.mRestorePurchasesListener.fail(result.getMessage());
                stopIabHelper(null);
            }

        }

        private void restorePurchasessFinished(List<IabPurchase> purchases) {
            mRestorePurchasesListener.success(purchases);
            stopIabHelper(null);
        }
    }

    /**
     * Handle Fetch Skus Details processes
     */
    private class FetchSkusDetailsFinishedListener implements IabHelper.FetchSkusDetailsFinishedListener {


        private IabCallbacks.OnFetchSkusDetailsListener mFetchSkusDetailsListener;

        public FetchSkusDetailsFinishedListener(IabCallbacks.OnFetchSkusDetailsListener fetchSkusDetailsListener) {
            this.mFetchSkusDetailsListener            = fetchSkusDetailsListener;
        }

        @Override
        public void onFetchSkusDetailsFinished(IabResult result, IabInventory inventory) {
            SoomlaUtils.LogDebug(TAG, "Restore Purchases succeeded");
            if (result.getResponse() == IabResult.BILLING_RESPONSE_RESULT_OK && mFetchSkusDetailsListener != null) {

                // @lassic (May 1st): actually, here (query finished) it only makes sense to get the details
                // of the SKUs we already queried for
                List<String> skuList = inventory.getAllQueriedSkus(false);
                List<IabSkuDetails> skuDetails = new ArrayList<IabSkuDetails>();
                for (String sku : skuList) {
                    IabSkuDetails skuDetail = inventory.getSkuDetails(sku);
                    if (skuDetail != null) {
                        skuDetails.add(skuDetail);
                    }
                }

                this.mFetchSkusDetailsListener.success(skuDetails);
            } else {
                SoomlaUtils.LogError(TAG, "Wither mFetchSkusDetailsListener==null OR Fetching details error: " + result.getMessage());
                if (this.mFetchSkusDetailsListener != null) this.mFetchSkusDetailsListener.fail(result.getMessage());
            }

            stopIabHelper(null);
        }
    }

    /**
     * Handle setup billing service process
     */
    private class OnIabSetupFinishedListener implements IabHelper.OnIabSetupFinishedListener {

        private IabCallbacks.IabInitListener mIabInitListener;

        public IabCallbacks.IabInitListener getIabInitListener() {
            return mIabInitListener;
        }

        public OnIabSetupFinishedListener(IabCallbacks.IabInitListener iabListener) {
            this.mIabInitListener = iabListener;
        }

        @Override
        public void onIabSetupFinished(IabResult result) {

            SoomlaUtils.LogDebug(TAG, "IAB helper Setup finished.");
            if (result.isFailure()) {
                if (mIabInitListener != null) mIabInitListener.fail(result.getMessage());
                return;
            }
            if (mIabInitListener != null) mIabInitListener.success(false);
        }
    }

    /**
     * Handle setup billing purchase process
     */
    private static class OnIabPurchaseFinishedListener implements IabHelper.OnIabPurchaseFinishedListener {

        public OnIabPurchaseFinishedListener() {
        }


        @Override
        public void onIabPurchaseFinished(IabResult result, final IabPurchase purchase) {
            /**
             * Wait to see if the purchase succeeded, then start the consumption process.
             */
            SoomlaUtils.LogDebug(TAG, "IabPurchase finished: " + result + ", purchase: " + purchase);

            GooglePlayIabService.getInstance().mWaitingServiceResponse = false;

            if (result.getResponse() == IabResult.BILLING_RESPONSE_RESULT_OK) {
                if (isVerifyPurchasesEnabled()) {
                    List<IabPurchase> purchases = Arrays.asList(purchase);
                    GooglePlayIabService.getInstance().mSavedOnPurchaseListener.verificationStarted(purchases);
                    GooglePlayIabService.getInstance().verifyPurchases(purchases, new VerifyPurchasesFinishedListener() {
                        @Override
                        public void finished() {
                            purchaseFinishedSuccessfully(purchase);
                        }
                    });
                } else {
                    purchaseFinishedSuccessfully(purchase);
                }
                return;
            } else if (result.getResponse() == IabResult.BILLING_RESPONSE_RESULT_USER_CANCELED) {
                IabCallbacks.OnPurchaseListener onPurchaseListener = GooglePlayIabService.getInstance().mSavedOnPurchaseListener;
                if (onPurchaseListener != null) {
                    onPurchaseListener.cancelled(purchase);
                }
            } else if (result.getResponse() == IabResult.BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED) {
                IabCallbacks.OnPurchaseListener onPurchaseListener = GooglePlayIabService.getInstance().mSavedOnPurchaseListener;
                if (onPurchaseListener != null) {
                    onPurchaseListener.alreadyOwned(purchase);
                }
            } else {
                IabCallbacks.OnPurchaseListener onPurchaseListener = GooglePlayIabService.getInstance().mSavedOnPurchaseListener;
                if (onPurchaseListener != null) {
                    onPurchaseListener.fail(result.getMessage());
                }
            }

            purchaseFinished();
        }

        private void purchaseFinishedSuccessfully(IabPurchase purchase) {
            IabCallbacks.OnPurchaseListener onPurchaseListener = GooglePlayIabService.getInstance().mSavedOnPurchaseListener;
            if (onPurchaseListener != null) {
                onPurchaseListener.success(purchase);
            }
            purchaseFinished();
        }

        private void purchaseFinished() {
            GooglePlayIabService.getInstance().mSavedOnPurchaseListener = null;
            GooglePlayIabService.getInstance().stopIabHelper(null);
        }
    }


    /**
     * Android In-App Billing v3 requires an activity to receive the result of the billing process.
     * This activity's job is to do just that, it also contains the white/green IAB window.
     * Please do NOT start it on your own.
     */
    public static class IabActivity extends Activity {
        private boolean mInProgressDestroy = false;

        @Override
        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);

            Intent intent = getIntent();
            String productId = intent.getStringExtra(SKU);
            String itemType = intent.getStringExtra(ITEM_TYPE);
            String payload = intent.getStringExtra(EXTRA_DATA);

            try {
                OnIabPurchaseFinishedListener onIabPurchaseFinishedListener = new OnIabPurchaseFinishedListener();
                GooglePlayIabService.getInstance().mHelper.launchPurchaseFlow(this, itemType, productId, onIabPurchaseFinishedListener, payload);
                GooglePlayIabService.getInstance().mWaitingServiceResponse = true;
            } catch (IllegalStateException e) {
                mInProgressDestroy = true;
                finish();
            } catch (Exception e) {
                SoomlaUtils.LogDebug(TAG, "MSG: " + e.getMessage());
                finish();

                String msg = "Error purchasing item " + e.getMessage();
                SoomlaUtils.LogError(TAG, msg);
                GooglePlayIabService.getInstance().mWaitingServiceResponse = false;

                IabCallbacks.OnPurchaseListener onPurchaseListener = GooglePlayIabService.getInstance().mSavedOnPurchaseListener;
                if (onPurchaseListener != null) {
                    onPurchaseListener.fail(msg);
                    GooglePlayIabService.getInstance().mSavedOnPurchaseListener = null;
                }
            }
        }

        @Override
        protected void onActivityResult(int requestCode, int resultCode, Intent data) {
            SoomlaUtils.LogDebug(TAG, "onActivityResult 1");
            if (!GooglePlayIabService.getInstance().mHelper.handleActivityResult(requestCode, resultCode, data)) {
                SoomlaUtils.LogDebug(TAG, "onActivityResult 2");
                super.onActivityResult(requestCode, resultCode, data);
            }
            SoomlaUtils.LogDebug(TAG, "onActivityResult 3");
            finish();
        }

        @Override
        protected void onPause() {
            super.onPause();
        }

        boolean firstTime = true;
        @Override
        protected void onResume() {
            SoomlaUtils.LogDebug(TAG, "onResume 1");
            super.onResume();
            firstTime = false;
        }

        @Override
        protected void onStop() {
            super.onStop();
        }

        @Override
        protected void onStart() {
            SoomlaUtils.LogDebug(TAG, "onStart 1");
            super.onStart();

            if (!firstTime && SoomlaApp.getAppContext() instanceof Activity) {
                SoomlaUtils.LogDebug(TAG, "onStart 2");
                onActivityResult(10001, Activity.RESULT_CANCELED, null);

                Intent tabIntent = new Intent(this, ((Activity) SoomlaApp.getAppContext()).getClass());
                tabIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                SoomlaUtils.LogDebug(TAG, "onStart 3");
                startActivity(tabIntent);
            }
            SoomlaUtils.LogDebug(TAG, "onStart 4");
        }

        @Override
        protected void onDestroy() {
            SoomlaUtils.LogDebug(TAG, "onDestroy 1");
            if (!mInProgressDestroy && GooglePlayIabService.getInstance().mWaitingServiceResponse)
            {
                SoomlaUtils.LogDebug(TAG, "onDestroy 2");
                GooglePlayIabService.getInstance().mWaitingServiceResponse = false;
                String err = "IabActivity is destroyed during purchase.";
                SoomlaUtils.LogError(TAG, err);

                // we're letting the helper take care of closing so there won't be any async process stuck in it.
                onActivityResult(10001, Activity.RESULT_CANCELED, null);

//                IabCallbacks.OnPurchaseListener onPurchaseListener = GooglePlayIabService.getInstance().mSavedOnPurchaseListener;
//                if (onPurchaseListener != null) {
//                    SoomlaUtils.LogDebug(TAG, "onDestroy 3");
//                    onPurchaseListener.fail(err);
//                    GooglePlayIabService.getInstance().mSavedOnPurchaseListener = null;
//                }
            }
            SoomlaUtils.LogDebug(TAG, "onDestroy 4");
            super.onDestroy();
        }
    }

    private void checkStringConfigItem(Map<String, Object> config, String key) {
        Object strToCheck = config.get(key);
        if (strToCheck == null || !(strToCheck instanceof String) || TextUtils.isEmpty((String) strToCheck)) {
            throw new IllegalArgumentException("Please, provide value for " + key);
        }
    }

    public static GooglePlayIabService getInstance() {
        return (GooglePlayIabService) SoomlaStore.getInstance().getInAppBillingService();
    }


    /* Private Members */
    private static final String TAG = "SOOMLA GooglePlayIabService";
    private GoogleIabHelper mHelper;
    private boolean keepIabServiceOpen = false;
    private boolean mWaitingServiceResponse = false;

    public static final String PUBLICKEY_KEY = "PO#SU#SO#GU";

    public static final String VERIFY_PURCHASES_KEY = "soomla.verification.verifyPurchases";
    public static final String VERIFY_ON_SERVER_FAILURE = "soomla.verification.verifyOnServerFailure";
    public static final String VERIFY_REFRESH_TOKEN_KEY = "soomla.verification.refreshToken";
    public static final String VERIFY_CLIENT_ID_KEY = "soomla.verification.clientId";
    public static final String VERIFY_CLIENT_SECRET_KEY = "soomla.verification.clientSecret";
    public static final String VERIFY_ACCESS_TOKEN_KEY = "soomla.verification.accessToken";

    private static final String SKU = "ID#sku";
    private static final String ITEM_TYPE = "ID#itemType";
    private static final String EXTRA_DATA = "ID#extraData";
    private IabCallbacks.OnPurchaseListener mSavedOnPurchaseListener = null;

    /**
     * When set to true, this removes the need to verify purchases when there's no signature.
     * This is useful while you are in development and testing stages of your game.
     *
     * WARNING: Do NOT publish your app with this set to true!!!
     */
    public static boolean AllowAndroidTestPurchases = false;
}
