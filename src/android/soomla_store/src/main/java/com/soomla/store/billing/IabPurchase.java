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

package com.soomla.store.billing;

import com.soomla.store.events.UnexpectedStoreErrorEvent;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Represents an in-app billing purchase.
 */
public class IabPurchase {
    private String mItemType;  // ITEM_TYPE_INAPP or ITEM_TYPE_SUBS
    private String mOrderId;
    private String mPackageName;
    private String mSku;
    private long mPurchaseTime;
    private int mPurchaseState;
    private String mDeveloperPayload;
    private String mToken;
    private String mOriginalJson;
    private String mSignature;
    private String mUserId; // NOTE: UserId is only used on Amazon Store.
    private boolean serverVerified = true;
    private UnexpectedStoreErrorEvent.ErrorCode verificationErrorCode;

    public IabPurchase(String itemType, String sku, String purchaseToken, String orderId, int purchaseState) {
        this(itemType, sku, purchaseToken, orderId, purchaseState, null);
    }

    public IabPurchase(String itemType, String sku, String purchaseToken, String orderId, int purchaseState, String userId) {
        mItemType = itemType;
        mSku = sku;
        mToken = purchaseToken;
        mOrderId = orderId;
        mPurchaseState = purchaseState;
        mUserId = userId;
    }

    public IabPurchase(String itemType, String jsonPurchaseInfo, String signature) throws JSONException {
        mItemType = itemType;
        mOriginalJson = jsonPurchaseInfo;
        JSONObject o = new JSONObject(mOriginalJson);
        mOrderId = o.optString("orderId");
        mPackageName = o.optString("packageName");
        mSku = o.optString("productId");
        mPurchaseTime = o.optLong("purchaseTime");
        mPurchaseState = o.optInt("purchaseState");
        mDeveloperPayload = o.optString("developerPayload");
        mToken = o.optString("token", o.optString("purchaseToken"));
        mSignature = signature;
    }

    public String getItemType() {
        return mItemType;
    }

    public String getOrderId() {
        return mOrderId;
    }

    public String getPackageName() {
        return mPackageName;
    }

    public String getSku() {
        return mSku;
    }

    public long getPurchaseTime() {
        return mPurchaseTime;
    }

    public int getPurchaseState() {
        return mPurchaseState;
    }

    public String getDeveloperPayload() {
        return mDeveloperPayload;
    }

    public String getToken() {
        return mToken;
    }

    public String getOriginalJson() {
        return mOriginalJson;
    }

    public String getSignature() {
        return mSignature;
    }

    public String getUserId() {
        return mUserId;
    }

    public boolean isServerVerified() {
        return serverVerified;
    }

    public void setServerVerified(boolean serverVerified) {
        this.serverVerified = serverVerified;
    }

    public void setDeveloperPayload(String developerPayload) {
        mDeveloperPayload = developerPayload;
    }

    public UnexpectedStoreErrorEvent.ErrorCode getVerificationErrorCode() {
        return verificationErrorCode;
    }

    public void setVerificationErrorCode(UnexpectedStoreErrorEvent.ErrorCode verificationErrorCode) {
        this.verificationErrorCode = verificationErrorCode;
    }

    @Override
    public String toString() { return "PurchaseInfo(type:" + mItemType + "):" + mOriginalJson; }
}
