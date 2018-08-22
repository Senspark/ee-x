/* Copyright (c) 2012 Google Inc.
 * Revised and edited by SOOMLA for stability and supporting new features.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.soomla.store.billing.google;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.pm.ResolveInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.text.TextUtils;
import com.android.vending.billing.IInAppBillingService;
import com.soomla.SoomlaApp;
import com.soomla.SoomlaConfig;
import com.soomla.SoomlaUtils;
import com.soomla.store.billing.IabException;
import com.soomla.store.billing.IabHelper;
import com.soomla.store.billing.IabInventory;
import com.soomla.store.billing.IabPurchase;
import com.soomla.store.billing.IabResult;
import com.soomla.store.billing.IabSkuDetails;
import org.json.JSONException;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;


/**
 * This is an implementation of SOOMLA's IabHelper to create a plugin of Google Play to SOOMLA.
 *
 * More docs in parent.
 */
public class GoogleIabHelper extends IabHelper {

    // uncomment to verify big chunk google bug (over 20)
//    public static final int SKU_QUERY_MAX_CHUNK_SIZE = 50;


    public static final int SKU_QUERY_MAX_CHUNK_SIZE = 19;

    // Keys for the responses from InAppBillingService
    public static final String RESPONSE_CODE = "RESPONSE_CODE";
    public static final String RESPONSE_GET_SKU_DETAILS_LIST = "DETAILS_LIST";
    public static final String RESPONSE_BUY_INTENT = "BUY_INTENT";
    public static final String RESPONSE_INAPP_PURCHASE_DATA = "INAPP_PURCHASE_DATA";
    public static final String RESPONSE_INAPP_SIGNATURE = "INAPP_DATA_SIGNATURE";
    public static final String RESPONSE_INAPP_ITEM_LIST = "INAPP_PURCHASE_ITEM_LIST";
    public static final String RESPONSE_INAPP_PURCHASE_DATA_LIST = "INAPP_PURCHASE_DATA_LIST";
    public static final String RESPONSE_INAPP_SIGNATURE_LIST = "INAPP_DATA_SIGNATURE_LIST";
    public static final String INAPP_CONTINUATION_TOKEN = "INAPP_CONTINUATION_TOKEN";

    // some fields on the getSkuDetails response bundle
    public static final String GET_SKU_DETAILS_ITEM_LIST = "ITEM_ID_LIST";
    public static final String GET_SKU_DETAILS_ITEM_TYPE_LIST = "ITEM_TYPE_LIST";

    /**
     * Creates an instance. After creation, it will not yet be ready to use. You must perform
     * setup by calling {@link #startSetup} and wait for setup to complete. This constructor does not
     * block and is safe to call from a UI thread.
     */
    public GoogleIabHelper() {
        SoomlaUtils.LogDebug(TAG, "GoogleIabHelper helper created.");
    }

    /**
     * See parent
     */
    protected void startSetupInner() {
        mServiceConn = new ServiceConnection() {
            @Override
            public void onServiceDisconnected(ComponentName name) {
                SoomlaUtils.LogDebug(TAG, "Billing service disconnected.");
                mService = null;
            }

            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                SoomlaUtils.LogDebug(TAG, "Billing service connected.");
                GoogleIabHelper.this.service = service;
                mService = IInAppBillingService.Stub.asInterface(service);
                String packageName = SoomlaApp.getAppContext().getPackageName();
                try {
                    SoomlaUtils.LogDebug(TAG, "Checking for in-app billing 3 support.");

                    // check for in-app billing v3 support
                    int inAppResponse = mService.isBillingSupported(3, packageName, ITEM_TYPE_INAPP);
                    int subsResponse = mService.isBillingSupported(3, packageName, ITEM_TYPE_SUBS);
                    if (inAppResponse != IabResult.BILLING_RESPONSE_RESULT_OK
                            || subsResponse != IabResult.BILLING_RESPONSE_RESULT_OK) {
                        setupFailed(new IabResult(inAppResponse != 0 ? inAppResponse : subsResponse, "Error checking for billing v3 support."));
                        return;
                    }
                    SoomlaUtils.LogDebug(TAG, "In-app billing version 3 supported for " + packageName);

                    setupSuccess();
                }
                catch (RemoteException e) {
                    setupFailed(new IabResult(IabResult.IABHELPER_REMOTE_EXCEPTION, "RemoteException while setting up in-app billing."));
                    e.printStackTrace();
                }
            }
        };

        Intent serviceIntent = new Intent("com.android.vending.billing.InAppBillingService.BIND");
        serviceIntent.setPackage("com.android.vending");
        List<ResolveInfo> intentServices = SoomlaApp.getAppContext().getPackageManager().queryIntentServices(serviceIntent, 0);
        if (intentServices != null && !intentServices.isEmpty()) {
            // service available to handle that Intent
            SoomlaApp.getAppContext().bindService(serviceIntent, mServiceConn, Context.BIND_AUTO_CREATE);
        }
        else {
            // no service available to handle that Intent
            setupFailed(new IabResult(IabResult.BILLING_RESPONSE_RESULT_BILLING_UNAVAILABLE, "Billing service unavailable on device."));
        }
    }

    /**
     * Dispose of object, releasing resources. It's very important to call this
     * method when you are done with this object. It will release any resources
     * used by it such as service connections. Naturally, once the object is
     * disposed of, it can't be used again.
     */
    public void dispose() {
        SoomlaUtils.LogDebug(TAG, "Disposing.");
        super.dispose();
        if (mServiceConn != null) {
            SoomlaUtils.LogDebug(TAG, "Unbinding from service.");
            if (SoomlaApp.getAppContext() != null && mService != null) SoomlaApp.getAppContext().unbindService(mServiceConn);
            mServiceConn = null;
            mService = null;
        }
    }

    /**
     * Handles an activity result that's part of the purchase flow in in-app billing. If you
     * are calling {@link #launchPurchaseFlow}, then you must call this method from your
     * Activity's {@link android.app.Activity@onActivityResult} method. This method
     * MUST be called from the UI thread of the Activity.
     *
     * @param requestCode The requestCode as you received it.
     * @param resultCode The resultCode as you received it.
     * @param data The data (Intent) as you received it.
     * @return Returns true if the result was related to a purchase flow and was handled;
     *     false if the result was not related to a purchase, in which case you should
     *     handle it normally.
     */
    public boolean handleActivityResult(int requestCode, int resultCode, Intent data) {
        IabResult result;
        if (requestCode != RC_REQUEST) return false;

        checkSetupDoneAndThrow("handleActivityResult");

        // cancellation moved here b/c there can be a cancellation with null data
        if (resultCode == Activity.RESULT_CANCELED) {
            SoomlaUtils.LogDebug(TAG, "IabPurchase canceled.");
            try {
                IabPurchase purchase = new IabPurchase(mPurchasingItemType, "{\"productId\":" + mPurchasingItemSku + "}", null);
                result = new IabResult(IabResult.BILLING_RESPONSE_RESULT_USER_CANCELED, "User canceled.");
                purchaseFailed(result, purchase);
                return true;
            } catch (JSONException e) {
                SoomlaUtils.LogError(TAG, "Failed to generate canceled purchase.");
                e.printStackTrace();
                result = new IabResult(IabResult.IABHELPER_BAD_RESPONSE, "Failed to generate canceled purchase.");
                purchaseFailed(result, null);
                return true;
            }
        }

        if (data == null) {
            SoomlaUtils.LogError(TAG, "Null data in IAB activity result.");
            result = new IabResult(IabResult.IABHELPER_BAD_RESPONSE, "Null data in IAB result");
            purchaseFailed(result, null);
            return true;
        }

        int responseCode = getResponseCodeFromIntent(data);
        String purchaseData = data.getStringExtra(RESPONSE_INAPP_PURCHASE_DATA);
        String dataSignature = data.getStringExtra(RESPONSE_INAPP_SIGNATURE);

        if (resultCode == Activity.RESULT_OK && responseCode == IabResult.BILLING_RESPONSE_RESULT_OK) {
            SoomlaUtils.LogDebug(TAG, "Successful resultcode from purchase activity.");
            SoomlaUtils.LogDebug(TAG, "IabPurchase data: " + purchaseData);
            SoomlaUtils.LogDebug(TAG, "Data signature: " + dataSignature);
            SoomlaUtils.LogDebug(TAG, "Extras: " + data.getExtras());
            SoomlaUtils.LogDebug(TAG, "Expected item type: " + mPurchasingItemType);

            if (purchaseData == null || dataSignature == null) {
                SoomlaUtils.LogError(TAG, "BUG: either purchaseData or dataSignature is null.");
                SoomlaUtils.LogDebug(TAG, "Extras: " + data.getExtras().toString());
                result = new IabResult(IabResult.IABHELPER_UNKNOWN_ERROR, "IAB returned null purchaseData or dataSignature");
                purchaseFailed(result, null);
                return true;
            }

            IabPurchase purchase = null;
            try {
                purchase = new IabPurchase(mPurchasingItemType, purchaseData, dataSignature);
                String sku = purchase.getSku();

                SharedPreferences prefs =
                        SoomlaApp.getAppContext().getSharedPreferences(SoomlaConfig.PREFS_NAME, Context.MODE_PRIVATE);
                String publicKey = prefs.getString(GooglePlayIabService.PUBLICKEY_KEY, "");

                // Verify signature
                if (!Security.verifyPurchase(publicKey, purchaseData, dataSignature)) {
                    SoomlaUtils.LogError(TAG, "IabPurchase signature verification FAILED for sku " + sku);
                    result = new IabResult(IabResult.IABHELPER_VERIFICATION_FAILED, "Signature verification failed for sku " + sku);
                    purchaseFailed(result, purchase);
                    return true;
                }
                SoomlaUtils.LogDebug(TAG, "IabPurchase signature successfully verified.");
            }
            catch (JSONException e) {
                SoomlaUtils.LogError(TAG, "Failed to parse purchase data.");
                e.printStackTrace();
                result = new IabResult(IabResult.IABHELPER_BAD_RESPONSE, "Failed to parse purchase data.");
                purchaseFailed(result, null);
                return true;
            }

            purchaseSucceeded(purchase);
        } else if (resultCode == Activity.RESULT_OK) {
            // result code was OK, but in-app billing response was not OK.
            SoomlaUtils.LogDebug(TAG, "Result code was OK but in-app billing response was not OK: " + IabResult.getResponseDesc(responseCode));
            result = new IabResult(responseCode, "Problem purchashing item.");
            purchaseFailed(result, null);
        }
        else {
            SoomlaUtils.LogError(TAG, "IabPurchase failed. Result code: " + Integer.toString(resultCode)
                    + ". Response: " + IabResult.getResponseDesc(responseCode));
            result = new IabResult(IabResult.IABHELPER_UNKNOWN_PURCHASE_RESPONSE, "Unknown purchase response.");
            purchaseFailed(result, null);
        }
        return true;
    }

    /**
     * Consumes a given in-app product. Consuming can only be done on an item
     * that's owned, and as a result of consumption, the user will no longer own it.
     * This method may block or take long to return. Do not call from the UI thread.
     * For that, see {@link #consumeAsync}.
     *
     * @param itemInfo The PurchaseInfo that represents the item to consume.
     * @throws IabException if there is a problem during consumption.
     */
     public void consume(IabPurchase itemInfo) throws IabException {
         checkSetupDoneAndThrow("consume");

        if (!itemInfo.getItemType().equals(ITEM_TYPE_INAPP)) {
            throw new IabException(IabResult.IABHELPER_INVALID_CONSUMPTION,
                    "Items of type '" + itemInfo.getItemType() + "' can't be consumed.");
        }

        try {
            String token = itemInfo.getToken();
            String sku = itemInfo.getSku();
            if (token == null || token.equals("")) {
               SoomlaUtils.LogError(TAG, "Can't consume "+ sku + ". No token.");
               throw new IabException(IabResult.IABHELPER_MISSING_TOKEN, "PurchaseInfo is missing token for sku: "
                   + sku + " " + itemInfo);
            }

            SoomlaUtils.LogDebug(TAG, "Consuming sku: " + sku + ", token: " + token);
            int response = mService.consumePurchase(3, SoomlaApp.getAppContext().getPackageName(), token);
            if (response == IabResult.BILLING_RESPONSE_RESULT_OK) {
               SoomlaUtils.LogDebug(TAG, "Successfully consumed sku: " + sku);
            }
            else {
               SoomlaUtils.LogDebug(TAG, "Error consuming sku " + sku + ". " + IabResult.getResponseDesc(response));
               throw new IabException(response, "Error consuming sku " + sku);
            }
        }
        catch (RemoteException e) {
            throw new IabException(IabResult.IABHELPER_REMOTE_EXCEPTION, "Remote exception while consuming. PurchaseInfo: " + itemInfo, e);
        }
    }



    /**
     * Asynchronous wrapper to item consumption. Works like {@link #consume}, but
     * performs the consumption in the background and notifies completion through
     * the provided listener. This method is safe to call from a UI thread.
     *
     * @param purchase The purchase to be consumed.
     * @param listener The listener to notify when the consumption operation finishes.
     */
    public void consumeAsync(IabPurchase purchase, OnConsumeFinishedListener listener) {
        checkSetupDoneAndThrow("consume");
        List<IabPurchase> purchases = new ArrayList<IabPurchase>();
        purchases.add(purchase);
        consumeAsyncInternal(purchases, listener, null);
    }

    /**
     * Same as {@link #consumeAsync}, but for multiple items at once.
     * @param purchases The list of PurchaseInfo objects representing the purchases to consume.
     * @param listener The listener to notify when the consumption operation finishes.
     */
    public void consumeAsync(List<IabPurchase> purchases, OnConsumeMultiFinishedListener listener) {
        checkSetupDoneAndThrow("consume");
        consumeAsyncInternal(purchases, null, listener);
    }

    /**
     * Callback that notifies when a consumption operation finishes.
     */
    public interface OnConsumeFinishedListener {
        /**
         * Called to notify that a consumption has finished.
         *
         * @param purchase The purchase that was (or was to be) consumed.
         * @param result The result of the consumption operation.
         */
        public void onConsumeFinished(IabPurchase purchase, IabResult result);
    }

    /**
     * Callback that notifies when a multi-item consumption operation finishes.
     */
    public interface OnConsumeMultiFinishedListener {
        /**
         * Called to notify that a consumption of multiple items has finished.
         *
         * @param purchases The purchases that were (or were to be) consumed.
         * @param results The results of each consumption operation, corresponding to each
         *     sku.
         */
        public void onConsumeMultiFinished(List<IabPurchase> purchases, List<IabResult> results);
    }


    /** Protected functions **/

    /**
     * see parent
     */
    @Override
    protected void restorePurchasesAsyncInner() {
        (new Thread(new Runnable() {
            public void run() {
                IabInventory inv = null;
                try {
                    inv = restorePurchases();
                }
                catch (IabException ex) {
                    IabResult result = ex.getResult();
                    restorePurchasesFailed(result);
                    return;
                }

                restorePurchasesSuccess(inv);
            }
        })).start();
    }

    /**
     * see parent
     */
    @Override
    protected void fetchSkusDetailsAsyncInner(final List<String> skus) {
        (new Thread(new Runnable() {
            public void run() {
                IabInventory inv = null;
                try {
                    inv = fetchSkusDetails(skus);
                }
                catch (IabException ex) {
                    IabResult result = ex.getResult();
                    fetchSkusDetailsFailed(result);
                    return;
                }

                fetchSkusDetailsSuccess(inv);
            }
        })).start();
    }

    /**
     * See parent
     */
    @Override
    protected void launchPurchaseFlowInner(Activity act, String itemType, String sku, String extraData) {
        IabResult result;

        if (!(itemType.equals(ITEM_TYPE_INAPP) || itemType.equals(ITEM_TYPE_SUBS))) {
            throw new IllegalArgumentException("Wrong purchase item type: " + itemType);
        }

        try {
            SoomlaUtils.LogDebug(TAG, "Constructing buy intent for " + sku + ", item type: " + itemType);
            Bundle buyIntentBundle = mService.getBuyIntent(3, SoomlaApp.getAppContext().getPackageName(), sku, itemType, extraData);
            buyIntentBundle.putString("PURCHASE_SKU", sku);
            int response = getResponseCodeFromBundle(buyIntentBundle);
            if (response != IabResult.BILLING_RESPONSE_RESULT_OK) {
                SoomlaUtils.LogError(TAG, "Unable to buy item, Error response: " + IabResult.getResponseDesc(response));

                IabPurchase failPurchase = new IabPurchase(itemType, "{\"productId\":" + sku + "}", null);
                result = new IabResult(response, "Unable to buy item");
                purchaseFailed(result, failPurchase);
                act.finish();
                return;
            }

            PendingIntent pendingIntent = buyIntentBundle.getParcelable(RESPONSE_BUY_INTENT);
            SoomlaUtils.LogDebug(TAG, "Launching buy intent for " + sku + ". Request code: " + RC_REQUEST);
            mPurchasingItemSku = sku;
            mPurchasingItemType = itemType;

            act.startIntentSenderForResult(pendingIntent.getIntentSender(),
                    RC_REQUEST, new Intent(),
                    Integer.valueOf(0), Integer.valueOf(0),
                    Integer.valueOf(0));
        } catch (SendIntentException e) {
            SoomlaUtils.LogError(TAG, "SendIntentException while launching purchase flow for sku " + sku);
            e.printStackTrace();

            result = new IabResult(IabResult.IABHELPER_SEND_INTENT_FAILED, "Failed to send intent.");
            purchaseFailed(result, null);
        } catch (RemoteException e) {
            SoomlaUtils.LogError(TAG, "RemoteException while launching purchase flow for sku " + sku);
            e.printStackTrace();

            result = new IabResult(IabResult.IABHELPER_REMOTE_EXCEPTION, "Remote exception while starting purchase flow");
            purchaseFailed(result, null);
        } catch (JSONException e) {
            SoomlaUtils.LogError(TAG, "Failed to generate failing purchase.");
            e.printStackTrace();

            result = new IabResult(IabResult.IABHELPER_BAD_RESPONSE, "Failed to generate failing purchase.");
            purchaseFailed(result, null);
        }


    }


    /** Private functions **/

    /**
     * The inner functions that consumes purchases.
     *
     * @param purchases the purchases to consume.
     * @param singleListener The listener to invoke when the consumption completes.
     * @param multiListener Multi listener for when we have multiple consumption operations.
     */
    private void consumeAsyncInternal(final List<IabPurchase> purchases,
                                      final OnConsumeFinishedListener singleListener,
                                      final OnConsumeMultiFinishedListener multiListener) {
        final Handler handler = new Handler();
        flagStartAsync("consume");
        (new Thread(new Runnable() {
            public void run() {
                final List<IabResult> results = new ArrayList<IabResult>();
                for (IabPurchase purchase : purchases) {
                    try {
                        consume(purchase);
                        results.add(new IabResult(IabResult.BILLING_RESPONSE_RESULT_OK, "Successful consume of sku " + purchase.getSku()));
                    }
                    catch (IabException ex) {
                        results.add(ex.getResult());
                    }
                }

                flagEndAsync();

                if (singleListener != null) {
                    handler.post(new Runnable() {
                        public void run() {
                            singleListener.onConsumeFinished(purchases.get(0), results.get(0));
                        }
                    });
                }
                if (multiListener != null) {
                    handler.post(new Runnable() {
                        public void run() {
                            multiListener.onConsumeMultiFinished(purchases, results);
                        }
                    });
                }
            }
        })).start();
    }

    /**
     * Queries the inventory. This will query specified skus' details from the server.
     *
     * @param skus additional PRODUCT skus to query information on, regardless of ownership.
     *     Ignored if null or if querySkuDetails is false.
     * @throws IabException if a problem occurs while refreshing the inventory.
     */
    private IabInventory fetchSkusDetails(List<String> skus) throws IabException {
        checkSetupDoneAndThrow("fetchSkusDetails");
        try {
            IabInventory inv = new IabInventory();
            int inAppResult = querySkuDetails(ITEM_TYPE_INAPP, inv, skus);
            int subsResult = querySkuDetails(ITEM_TYPE_SUBS, inv, skus);
            if (inAppResult != IabResult.BILLING_RESPONSE_RESULT_OK
                    || subsResult != IabResult.BILLING_RESPONSE_RESULT_OK) {
                throw new IabException(inAppResult != inAppResult ? inAppResult : subsResult, "Error refreshing inventory (querying prices of items).");
            }

            return inv;
        }
        catch (RemoteException e) {
            throw new IabException(IabResult.IABHELPER_REMOTE_EXCEPTION, "Remote exception while refreshing inventory.", e);
        }
        catch (JSONException e) {
            throw new IabException(IabResult.IABHELPER_BAD_RESPONSE, "Error parsing JSON response while refreshing inventory.", e);
        }
    }

    /**
     * Restores purchases from Google Play.
     *
     * @throws JSONException
     * @throws RemoteException
     */
    private int queryPurchases(IabInventory inv, String itemType) throws JSONException, RemoteException {
        // Query purchases
        SoomlaUtils.LogDebug(TAG, "Querying owned items, item type: " + itemType);
        SoomlaUtils.LogDebug(TAG, "Package name: " + SoomlaApp.getAppContext().getPackageName());
        boolean verificationFailed = false;
        String continueToken = null;

        do {
            SoomlaUtils.LogDebug(TAG, "Calling getPurchases with continuation token: " + continueToken);
            if (mService == null) {
                if (service != null) {
                    mService = IInAppBillingService.Stub.asInterface(service);
                } else {
                    return IabResult.IABHELPER_VERIFICATION_FAILED;
                }
            }
            Bundle ownedItems = mService.getPurchases(3,
                    SoomlaApp.getAppContext().getPackageName(),
                    itemType, continueToken);

            int response = getResponseCodeFromBundle(ownedItems);
            SoomlaUtils.LogDebug(TAG, "Owned items response: " + String.valueOf(response));
            if (response != IabResult.BILLING_RESPONSE_RESULT_OK) {
                SoomlaUtils.LogDebug(TAG, "getPurchases() failed: " + IabResult.getResponseDesc(response));
                return response;
            }
            if (!ownedItems.containsKey(RESPONSE_INAPP_ITEM_LIST)
                    || !ownedItems.containsKey(RESPONSE_INAPP_PURCHASE_DATA_LIST)
                    || !ownedItems.containsKey(RESPONSE_INAPP_SIGNATURE_LIST)) {
                SoomlaUtils.LogError(TAG, "Bundle returned from getPurchases() doesn't contain required fields.");
                return IabResult.IABHELPER_BAD_RESPONSE;
            }

            ArrayList<String> ownedSkus = ownedItems.getStringArrayList(
                    RESPONSE_INAPP_ITEM_LIST);
            ArrayList<String> purchaseDataList = ownedItems.getStringArrayList(
                    RESPONSE_INAPP_PURCHASE_DATA_LIST);
            ArrayList<String> signatureList = ownedItems.getStringArrayList(
                    RESPONSE_INAPP_SIGNATURE_LIST);

            SharedPreferences prefs =
                    SoomlaApp.getAppContext().getSharedPreferences(SoomlaConfig.PREFS_NAME, Context.MODE_PRIVATE);
            String publicKey = prefs.getString(GooglePlayIabService.PUBLICKEY_KEY, "");
            for (int i = 0; i < purchaseDataList.size(); ++i) {
                String purchaseData = purchaseDataList.get(i);
                String signature = signatureList.get(i);
                String sku = ownedSkus.get(i);
                if (Security.verifyPurchase(publicKey, purchaseData, signature)) {
                    SoomlaUtils.LogDebug(TAG, "Sku is owned: " + sku);
                    IabPurchase purchase = new IabPurchase(itemType, purchaseData, signature);

                    if (TextUtils.isEmpty(purchase.getToken())) {
                        SoomlaUtils.LogWarning(TAG, "BUG: empty/null token!");
                        SoomlaUtils.LogDebug(TAG, "IabPurchase data: " + purchaseData);
                    }

                    // Record ownership and token
                    inv.addPurchase(purchase);
                }
                else {
                    SoomlaUtils.LogWarning(TAG, "IabPurchase signature verification **FAILED**. Not adding item.");
                    SoomlaUtils.LogDebug(TAG, "   IabPurchase data: " + purchaseData);
                    SoomlaUtils.LogDebug(TAG, "   Signature: " + signature);
                    verificationFailed = true;
                }
            }

            continueToken = ownedItems.getString(INAPP_CONTINUATION_TOKEN);
            SoomlaUtils.LogDebug(TAG, "Continuation token: " + continueToken);
        } while (!TextUtils.isEmpty(continueToken));

        return verificationFailed ? IabResult.IABHELPER_VERIFICATION_FAILED : IabResult.BILLING_RESPONSE_RESULT_OK;
    }

    /**
     * Retrieves all items that were purchase but not consumed.
     *
     * @throws IabException
     */
    private IabInventory restorePurchases() throws IabException {
        checkSetupDoneAndThrow("restorePurchases");
        try {
            IabInventory inv = new IabInventory();
            int inAppResult = queryPurchases(inv, ITEM_TYPE_INAPP);
            int subsResult = queryPurchases(inv, ITEM_TYPE_SUBS);
            if (inAppResult != IabResult.BILLING_RESPONSE_RESULT_OK
                    || subsResult != IabResult.BILLING_RESPONSE_RESULT_OK) {
                throw new IabException(inAppResult != 0 ? inAppResult : subsResult, "Error refreshing inventory (querying owned items).");
            }
            return inv;
        }
        catch (RemoteException e) {
            throw new IabException(IabResult.IABHELPER_REMOTE_EXCEPTION, "Remote exception while refreshing inventory.", e);
        }
        catch (JSONException e) {
            throw new IabException(IabResult.IABHELPER_BAD_RESPONSE, "Error parsing JSON response while refreshing inventory.", e);
        }
    }

    /**
     * Fetches items details for a given list of items.
     *
     * @throws RemoteException
     * @throws JSONException
     */
    private int querySkuDetails(String itemType, IabInventory inv, List<String> skus)
            throws RemoteException, JSONException {
        SoomlaUtils.LogDebug(TAG, "Querying SKU details.");

        // a list here is a bug no matter what, there is no point in
        // querying duplicates, and it can only create other bugs
        // on top of degrading performance
        // however, we need a subList later for chunks, so just
        // make the list through a Set 'filter'
        Set<String> skuSet = new HashSet<String>(skus);
        ArrayList<String> skuList = new ArrayList<String>(skuSet);

        if (skuList.size() == 0) {
            SoomlaUtils.LogDebug(TAG, "queryPrices: nothing to do because there are no SKUs.");
            return IabResult.BILLING_RESPONSE_RESULT_OK;
        }

        // see: http://stackoverflow.com/a/21080893/1469004
        int chunkIndex = 1;
        while (skuList.size() > 0) {
            ArrayList<String> skuSubList = new ArrayList<String>(
                    skuList.subList(0, Math.min(SKU_QUERY_MAX_CHUNK_SIZE, skuList.size())));
            skuList.removeAll(skuSubList);
            final int chunkResponse = querySkuDetailsChunk(itemType, inv, skuSubList);
            if (chunkResponse != IabResult.BILLING_RESPONSE_RESULT_OK) {
                // todo: TBD skip chunk or abort?
                // for now aborting at that point
                SoomlaUtils.LogDebug(TAG, String.format("querySkuDetails[chunk=%d] failed: %s",
                        chunkIndex, IabResult.getResponseDesc(chunkResponse)));
                return chunkResponse; // ABORT
            }
            chunkIndex++;
        }

        return IabResult.BILLING_RESPONSE_RESULT_OK;
    }

    /**
     * Queries a chunk of SKU details to prevent Google's 20 items bug.
     *
     * @throws RemoteException
     * @throws JSONException
     */
    private int querySkuDetailsChunk(String itemType, IabInventory inv, ArrayList<String> chunkSkuList) throws RemoteException, JSONException {
        Bundle querySkus = new Bundle();
        querySkus.putStringArrayList(GET_SKU_DETAILS_ITEM_LIST, chunkSkuList);
        Bundle skuDetails = mService.getSkuDetails(3, SoomlaApp.getAppContext().getPackageName(),
                itemType, querySkus);

        if (!skuDetails.containsKey(RESPONSE_GET_SKU_DETAILS_LIST)) {
            int response = getResponseCodeFromBundle(skuDetails);
            if (response != IabResult.BILLING_RESPONSE_RESULT_OK) {
                SoomlaUtils.LogDebug(TAG, "querySkuDetailsChunk() failed: " + IabResult.getResponseDesc(response));
                return response;
            }
            else {
                SoomlaUtils.LogError(TAG, "querySkuDetailsChunk() returned a bundle with neither an error nor a detail list.");
                return IabResult.IABHELPER_BAD_RESPONSE;
            }
        }

        ArrayList<String> responseList = skuDetails.getStringArrayList(
                RESPONSE_GET_SKU_DETAILS_LIST);

        for (String thisResponse : responseList) {
            IabSkuDetails d = new IabSkuDetails(itemType, thisResponse);
            SoomlaUtils.LogDebug(TAG, "Got sku details: " + d);
            inv.addSkuDetails(d);
        }

        return IabResult.BILLING_RESPONSE_RESULT_OK;
    }

    /**
     * Workaround to bug where sometimes response codes come as Long instead of Integer
     */
    private int getResponseCodeFromBundle(Bundle b) {
        Object o = b.get(RESPONSE_CODE);
        if (o == null) {
            SoomlaUtils.LogDebug(TAG, "Bundle with null response code, assuming OK (known issue)");
            return IabResult.BILLING_RESPONSE_RESULT_OK;
        }
        else if (o instanceof Integer) return ((Integer)o).intValue();
        else if (o instanceof Long) return (int)((Long)o).longValue();
        else {
            SoomlaUtils.LogError(TAG, "Unexpected type for bundle response code.");
            SoomlaUtils.LogError(TAG, o.getClass().getName());
            throw new RuntimeException("Unexpected type for bundle response code: " + o.getClass().getName());
        }
    }

    /**
     * Workaround to bug where sometimes response codes come as Long instead of Integer
     */
    private int getResponseCodeFromIntent(Intent i) {
        Object o = i.getExtras().get(RESPONSE_CODE);
        if (o == null) {
            SoomlaUtils.LogError(TAG, "Intent with no response code, assuming OK (known issue)");
            return IabResult.BILLING_RESPONSE_RESULT_OK;
        }
        else if (o instanceof Integer) return ((Integer)o).intValue();
        else if (o instanceof Long) return (int)((Long)o).longValue();
        else {
            SoomlaUtils.LogError(TAG, "Unexpected type for intent response code.");
            SoomlaUtils.LogError(TAG, o.getClass().getName());
            throw new RuntimeException("Unexpected type for intent response code: " + o.getClass().getName());
        }
    }

    /** Private Members **/

    private static String TAG = "SOOMLA GoogleIabHelper";

    // Connection to the service
    private IInAppBillingService mService;
    private ServiceConnection mServiceConn;
    private IBinder service;

    // The item type of the current purchase flow
    private String mPurchasingItemType;

    // The SKU of the item in the current purchase flow
    private String mPurchasingItemSku;

    private static final int RC_REQUEST = 10001;

}
