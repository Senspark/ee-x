package com.soomla.cocos2dx.store;

import android.opengl.GLSurfaceView;
import com.soomla.BusProvider;
import com.soomla.cocos2dx.common.NdkGlue;
import com.soomla.store.domain.MarketItem;
import com.soomla.store.events.*;
import com.squareup.otto.Subscribe;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

/**
 * This bridge is used to populate events from the store to cocos2dx (through JNI).
 */
public class StoreEventHandlerBridge {

    private GLSurfaceView mGLThread;

    public StoreEventHandlerBridge() {
        BusProvider.getInstance().register(this);
    }

    public void pushOnItemPurchased(String itemId, String payload) {
        BusProvider.getInstance().post(new ItemPurchasedEvent(itemId, payload, this));
    }

    public void pushOnItemPurchaseStarted(String itemId) {
        BusProvider.getInstance().post(new ItemPurchaseStartedEvent(itemId, this));
    }

    public void pushOnUnexpectedStoreError(int errorCode) {
        BusProvider.getInstance().post(new UnexpectedStoreErrorEvent(
                UnexpectedStoreErrorEvent.ErrorCode.values()[errorCode], this));
    }

    public void pushOnSoomlaStoreInitialized() {
        BusProvider.getInstance().post(new SoomlaStoreInitializedEvent(this));
    }

    @Subscribe
    public void onBillingNotSupported(BillingNotSupportedEvent billingNotSupportedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onBillingNotSupported");
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onBillingSupported(BillingSupportedEvent billingSupportedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onBillingSupported");
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }
	
    @Subscribe
    public void onIabServiceStarted(IabServiceStartedEvent iabServiceStartedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onIabServiceStarted");
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }
	
    @Subscribe
    public void onIabServiceStopped(IabServiceStoppedEvent iabServiceStoppedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onIabServiceStopped");
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onCurrencyBalanceChanged(final CurrencyBalanceChangedEvent currencyBalanceChangedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onCurrencyBalanceChanged");
                    parameters.put("itemId", currencyBalanceChangedEvent.getCurrencyItemId());
                    parameters.put("balance", currencyBalanceChangedEvent.getBalance());
                    parameters.put("amountAdded", currencyBalanceChangedEvent.getAmountAdded());
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onGoodBalanceChanged(final GoodBalanceChangedEvent goodBalanceChangedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onGoodBalanceChanged");
                    parameters.put("itemId", goodBalanceChangedEvent.getGoodItemId());
                    parameters.put("balance", goodBalanceChangedEvent.getBalance());
                    parameters.put("amountAdded", goodBalanceChangedEvent.getAmountAdded());
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onGoodEquipped(final GoodEquippedEvent goodEquippedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onGoodEquipped");
                    parameters.put("itemId", goodEquippedEvent.getGoodItemId());
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onGoodUnequipped(final GoodUnEquippedEvent goodUnEquippedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onGoodUnEquipped");
                    parameters.put("itemId", goodUnEquippedEvent.getGoodItemId());
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onGoodUpgrade(final GoodUpgradeEvent goodUpgradeEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onGoodUpgrade");
                    parameters.put("itemId", goodUpgradeEvent.getGoodItemId());
                    parameters.put("vguItemId", goodUpgradeEvent.getCurrentUpgrade());
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onItemPurchased(final ItemPurchasedEvent itemPurchasedEvent) {
        if (itemPurchasedEvent.Sender == this) {
            return;
        }
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onItemPurchased");
                    parameters.put("itemId", itemPurchasedEvent.getItemId());
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onItemPurchaseStarted(final ItemPurchaseStartedEvent itemPurchaseStartedEvent) {
        if (itemPurchaseStartedEvent.Sender == this) {
            return;
        }
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onItemPurchaseStarted");
                    parameters.put("itemId", itemPurchaseStartedEvent.getItemId());
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onMarketPurchaseCancelled(final MarketPurchaseCancelledEvent marketPurchaseCancelledEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onMarketPurchaseCancelled");
                    parameters.put("itemId", marketPurchaseCancelledEvent.getPurchasableVirtualItem().getItemId());
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onMarketPurchase(final MarketPurchaseEvent marketPurchaseEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onMarketPurchase");
                    parameters.put("itemId", marketPurchaseEvent.PurchasableVirtualItem.getItemId());
                    parameters.put("payload", marketPurchaseEvent.Payload);
                    if (marketPurchaseEvent.ExtraInfo != null) {
                        JSONObject extraInfo = new JSONObject();
                        for (Map.Entry<String, String> entry : marketPurchaseEvent.ExtraInfo.entrySet()) {
                            extraInfo.put(entry.getKey(), entry.getValue());
                        }
                        parameters.put("extraInfo", extraInfo);
                    }
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onMarketPurchaseStarted(final MarketPurchaseStartedEvent marketPurchaseStartedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onMarketPurchaseStarted");
                    parameters.put("itemId", marketPurchaseStartedEvent.getPurchasableVirtualItem().getItemId());
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onVerificationStarted(final VerificationStartedEvent verificationStartedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("itemId", verificationStartedEvent.getPurchasableVirtualItem().getItemId());
                    parameters.put("method", "CCStoreEventHandler::onVerificationStarted");
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onMarketItemsRefreshStarted(final MarketItemsRefreshStartedEvent marketItemsRefreshStartedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onMarketItemsRefreshStarted");
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onMarketItemsRefreshFailed(final MarketItemsRefreshFailedEvent marketItemsRefreshFailedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onMarketItemsRefreshFailed");
                    parameters.put("errorMessage", marketItemsRefreshFailedEvent.ErrorMessage);
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onMarketItemsRefreshed(final MarketItemsRefreshFinishedEvent marketItemsRefreshed) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONArray marketItemsJson = new JSONArray();
                    JSONObject marketItemJson;

                    for (MarketItem marketItem : marketItemsRefreshed.getMarketItems()) {
                        marketItemJson = new JSONObject();
                        marketItemJson.put("productId", marketItem.getProductId());
                        marketItemJson.put("marketPrice", marketItem.getMarketPriceAndCurrency());
                        marketItemJson.put("marketTitle", marketItem.getMarketTitle());
                        marketItemJson.put("marketDesc", marketItem.getMarketDescription());
                        marketItemJson.put("marketCurrencyCode", marketItem.getMarketCurrencyCode());
                        marketItemJson.put("marketPriceMicros", marketItem.getMarketPriceMicros());
                        marketItemsJson.put(marketItemJson);
                    }
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onMarketItemsRefreshed");
                    parameters.put("marketItems", marketItemsJson);
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onMarketRefund(final MarketRefundEvent playRefundEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onMarketRefund");
                    parameters.put("itemId", playRefundEvent.getPurchasableVirtualItem().getItemId());
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onRestoreTransactionsFinished(final RestoreTransactionsFinishedEvent restoreTransactionsEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onRestoreTransactionsFinished");
                    parameters.put("success", restoreTransactionsEvent.isSuccess());
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onRestoreTransactionsStarted(final RestoreTransactionsStartedEvent restoreTransactionsStartedEvent) {
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onRestoreTransactionsStarted");
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onUnexpectedStoreError(final UnexpectedStoreErrorEvent unexpectedStoreErrorEvent) {
        if (unexpectedStoreErrorEvent.Sender == this) {
            return;
        }
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onUnexpectedStoreError");
                    parameters.put("errorCode", unexpectedStoreErrorEvent.getErrorCode().ordinal());
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    @Subscribe
    public void onStoreStoreInitialized(SoomlaStoreInitializedEvent soomlaStoreInitializedEvent) {
        if (soomlaStoreInitializedEvent.Sender == this) {
            return;
        }
        mGLThread.queueEvent(new Runnable() {
            @Override
            public void run() {
                try {
                    JSONObject parameters = new JSONObject();
                    parameters.put("method", "CCStoreEventHandler::onSoomlaStoreInitialized");
                    NdkGlue.getInstance().sendMessageWithParameters(parameters);
                } catch (JSONException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    public void setGlSurfaceView(GLSurfaceView glSurfaceView) {
        this.mGLThread = glSurfaceView;
    }
}
