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

package com.soomla.store.domain.virtualGoods;

import com.soomla.SoomlaUtils;
import com.soomla.store.data.StorageManager;
import com.soomla.store.data.StoreInfo;
import com.soomla.store.data.StoreJSONConsts;
import com.soomla.store.domain.PurchasableVirtualItem;
import com.soomla.store.domain.VirtualItem;
import com.soomla.store.exceptions.VirtualItemNotFoundException;
import com.soomla.store.purchaseTypes.PurchaseType;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Iterator;

/**
 * SingleUsePacks are just bundles of <code>SingleUseVG</code>'s.
 * This kind of virtual good can be used to let your users buy more than one SingleUseVG at once.
 *
 * The SingleUsePackVG's characteristics are:
 *  1. Can be purchased an unlimited number of times.
 *  2. Doesn't have a balance in the database. The SingleUseVG that's associated with this pack
 *     has its own balance. When your users buy a SingleUsePackVG, the balance of the associated
 *     SingleUseVG goes up in the amount that this pack represents (mGoodAmount).
 *
 * Real Game Examples: 'Box Of Chocolates', '10 Swords'
 *
 * NOTE: In case you want this item to be available for purchase in the market (PurchaseWithMarket),
 * you will need to define the item in the market (Google Play, Amazon App Store, etc...).
 *
 * Inheritance: SingleUsePackVG >
 * {@link VirtualGood} >
 * {@link PurchasableVirtualItem} >
 * {@link VirtualItem}
 */
public class SingleUsePackVG extends VirtualGood {

    /** Constructor
     *
     * @param goodItemId the itemId of the SingleUseVG associated with this pack.
     * @param amount the number of SingleUseVGs in the pack.
     * @param name see parent
     * @param description see parent
     * @param itemId see parent
     * @param purchaseType see parent
     */
    public SingleUsePackVG(String goodItemId, int amount,
                           String name, String description,
                           String itemId, PurchaseType purchaseType) {
        super(name, description, itemId, purchaseType);

        mGoodItemId = goodItemId;
        mGoodAmount = amount;
    }

    /**
     * Constructor
     *
     * @param jsonObject see parent
     * @throws JSONException
     */
    public SingleUsePackVG(JSONObject jsonObject) throws JSONException {
        super(jsonObject);

        mGoodItemId = jsonObject.getString(StoreJSONConsts.VGP_GOOD_ITEMID);
        mGoodAmount = jsonObject.getInt(StoreJSONConsts.VGP_GOOD_AMOUNT);
    }

    /**
     * @{inheritDoc}
     */
    @Override
    public JSONObject toJSONObject() {
        JSONObject parentJsonObject = super.toJSONObject();
        JSONObject jsonObject = new JSONObject();
        try {
            Iterator<?> keys = parentJsonObject.keys();
            while(keys.hasNext())
            {
                String key = (String)keys.next();
                jsonObject.put(key, parentJsonObject.get(key));
            }

            jsonObject.put(StoreJSONConsts.VGP_GOOD_ITEMID, mGoodItemId);
            jsonObject.put(StoreJSONConsts.VGP_GOOD_AMOUNT, mGoodAmount);
        } catch (JSONException e) {
            SoomlaUtils.LogError(TAG, "An error occurred while generating JSON object.");
        }

        return jsonObject;
    }

    /**
     * @{inheritDoc}
     */
    @Override
    public int give(int amount, boolean notify) {
        SingleUseVG good = null;
        try {
            good = (SingleUseVG) StoreInfo.getVirtualItem(mGoodItemId);
        } catch (VirtualItemNotFoundException e) {
            SoomlaUtils.LogError(TAG, "SingleUseVG with itemId: " + mGoodItemId + " doesn't exist! Can't give this pack.");
            return 0;
        }
        return StorageManager.getVirtualGoodsStorage().add(good.getItemId(), mGoodAmount*amount, notify);
    }

    /**
     * @{inheritDoc}
     */
    @Override
    public int take(int amount, boolean notify) {
        SingleUseVG good = null;
        try {
            good = (SingleUseVG)StoreInfo.getVirtualItem(mGoodItemId);
        } catch (VirtualItemNotFoundException e) {
            SoomlaUtils.LogError(TAG, "SingleUseVG with itemId: " + mGoodItemId + " doesn't exist! Can't take this pack.");
            return 0;
        }
        return StorageManager.getVirtualGoodsStorage().remove(good.getItemId(), mGoodAmount*amount, notify);
    }

    /**
     * @{inheritDoc}
     */
    @Override
    protected boolean canBuy() {
        return true;
    }


    /** Setters and Getters **/

    public String getGoodItemId() {
        return mGoodItemId;
    }

    public int getGoodAmount() {
        return mGoodAmount;
    }


    /** Private Members **/

    private static final String TAG = "SOOMLA SingleUsePackVG"; //used for Log messages

    private String mGoodItemId; //the itemId of the SingleUseVG associated with this Pack.

    private int mGoodAmount; //the number of SingleUseVGs in the pack.
}
