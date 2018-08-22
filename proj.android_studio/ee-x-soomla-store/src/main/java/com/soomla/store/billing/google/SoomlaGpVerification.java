package com.soomla.store.billing.google;

import android.content.Context;
import android.content.SharedPreferences;
import android.text.TextUtils;

import com.soomla.SoomlaApp;
import com.soomla.SoomlaUtils;
import com.soomla.store.billing.IabPurchase;
import com.soomla.store.events.UnexpectedStoreErrorEvent;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * @author vedi
 *         date 26/05/15
 */
public class SoomlaGpVerification {

    private static final String VERIFY_URL = "https://iap.vedidev.com/verify_android";
    private static final String GOOGLE_AUTH_URL = "https://accounts.google.com/o/oauth2/token";
    private static final String TAG = "SOOMLA SoomlaGpVerification";

    private final IabPurchase purchase;
    private final String clientId;
    private final String clientSecret;
    private final String refreshToken;
    private final boolean verifyOnServerFailure;
    private String accessToken = null;

    public SoomlaGpVerification(IabPurchase purchase, String clientId, String clientSecret, String refreshToken, boolean verifyOnServerFailure) {
        if (purchase == null || TextUtils.isEmpty(clientId) || TextUtils.isEmpty(clientSecret) || TextUtils.isEmpty(refreshToken)) {
            SoomlaUtils.LogError(TAG, "Can't initialize SoomlaGpVerification. Missing params.");
            throw new IllegalArgumentException();
        }

        this.clientId = clientId;
        this.clientSecret = clientSecret;
        this.refreshToken = refreshToken;
        this.verifyOnServerFailure = verifyOnServerFailure;

        this.purchase = purchase;
    }

    private HttpResponse doVerifyPost(JSONObject jsonObject) throws IOException {
        HttpClient client = new DefaultHttpClient();
        HttpPost post = new HttpPost(VERIFY_URL);
        post.setHeader("Content-type", "application/json");

        String body = jsonObject.toString();
        post.setEntity(new StringEntity(body, "UTF8"));
        return client.execute(post);
    }

    public void verifyPurchase() {
        boolean verified = SoomlaGpVerification.this.verifyOnServerFailure;

        UnexpectedStoreErrorEvent.ErrorCode errorCode = UnexpectedStoreErrorEvent.ErrorCode.VERIFICATION_TIMEOUT;

        try {
            if (refreshToken()) {

                if (TextUtils.isEmpty(accessToken)) {
                    throw new IllegalStateException();
                }

                IabPurchase purchase = SoomlaGpVerification.this.purchase;

                String purchaseToken = purchase.getToken();

                if (purchaseToken != null) {
                    JSONObject jsonObject = new JSONObject();
                    jsonObject.put("purchaseToken", purchaseToken);
                    jsonObject.put("packageName", purchase.getPackageName());
                    jsonObject.put("productId", purchase.getSku());
                    jsonObject.put("accessToken", accessToken);
                    SoomlaUtils.LogDebug(TAG, String.format("verifying purchase on server: %s", VERIFY_URL));
                    SoomlaUtils.LogDebug(TAG, "purchase details: sku = " + purchase.getSku() + " token = " + purchaseToken);

                    SharedPreferences prefs = SoomlaApp.getAppContext().
                            getSharedPreferences("store.verification.prefs", Context.MODE_PRIVATE);
                    Map<String, ?> extraData = prefs.getAll();
                    if (extraData != null && !extraData.keySet().isEmpty()) {
                        for (String key : extraData.keySet()) {
                            jsonObject.put(key, extraData.get(key));
                        }
                    }

                    HttpResponse resp = doVerifyPost(jsonObject);

                    if (resp != null) {
                        int statusCode = resp.getStatusLine().getStatusCode();

                        StringBuilder stringBuilder = new StringBuilder();
                        InputStream inputStream = resp.getEntity().getContent();
                        Reader reader = new BufferedReader(new InputStreamReader(inputStream));
                        final char[] buffer = new char[1024];
                        int bytesRead;
                        while ((bytesRead = reader.read(buffer, 0, buffer.length)) > 0) {
                            stringBuilder.append(buffer, 0, bytesRead);
                        }
                        JSONObject resultJsonObject = new JSONObject(stringBuilder.toString());
                        if (statusCode >= 200 && statusCode <= 299) {
                            verified = resultJsonObject.optBoolean("verified", false);
                            if (verified) {
                                errorCode = null;
                            } else {
                                errorCode = UnexpectedStoreErrorEvent.ErrorCode.VERIFICATION_FAIL;
                                SoomlaUtils.LogError(TAG, "Failed to verify transaction receipt. The user will not get what he just bought.");
                                try {
                                    SoomlaUtils.LogError(TAG, "Reason: " + resultJsonObject.getJSONObject("reason").getJSONObject("error").getString("message"));
                                } catch (JSONException jsonException) {
                                    //nothing to to, just can't find required field in JSON
                                }
                            }
                        } else {
                        SoomlaUtils.LogError(TAG, "An error occurred while trying to get receipt purchaseToken. " +
                                "Stopping the purchasing process for: " + SoomlaGpVerification.this.purchase.getSku());
                        }
                    } else {
                        SoomlaUtils.LogError(TAG, "Got null response");
                    }
                } else {
                    SoomlaUtils.LogError(TAG, "An error occurred while trying to get receipt purchaseToken. " +
                            "Stopping the purchasing process for: " + SoomlaGpVerification.this.purchase.getSku());
                }
            } else {
                SoomlaUtils.LogError(TAG, "Cannot refresh token");
            }

        } catch (JSONException e) {
            SoomlaUtils.LogError(TAG, "Cannot build up json for verification: " + e);
        } catch (IOException e) {
            SoomlaUtils.LogError(TAG, e.getMessage());
        }

        purchase.setServerVerified(verified);
        purchase.setVerificationErrorCode(errorCode);
    }

    private boolean refreshToken() throws IOException, JSONException {
        this.accessToken = null;
        HttpClient client = new DefaultHttpClient();
        HttpPost post = new HttpPost(GOOGLE_AUTH_URL);

        List<NameValuePair> urlParameters = new ArrayList<NameValuePair>();
        urlParameters.add(new BasicNameValuePair("grant_type", "refresh_token"));
        urlParameters.add(new BasicNameValuePair("client_id", clientId));
        urlParameters.add(new BasicNameValuePair("client_secret", clientSecret));
        urlParameters.add(new BasicNameValuePair("refresh_token", refreshToken));

        post.setEntity(new UrlEncodedFormEntity(urlParameters));

        HttpResponse resp = client.execute(post);

        if (resp == null) {
            SoomlaUtils.LogError(TAG, "Failed to connect to google server.");
            return false;
        }

        StringBuilder stringBuilder = new StringBuilder();
        InputStream inputStream = resp.getEntity().getContent();
        Reader reader = new BufferedReader(new InputStreamReader(inputStream));
        final char[] buffer = new char[1024];
        int bytesRead;
        while ((bytesRead = reader.read(buffer, 0, buffer.length)) > 0) {
            stringBuilder.append(buffer, 0, bytesRead);
        }
        JSONObject resultJsonObject = new JSONObject(stringBuilder.toString());

        int statusCode = resp.getStatusLine().getStatusCode();
        if (statusCode < 200 || statusCode > 299) {
            SoomlaUtils.LogError(TAG, "There was a problem refreshing the token. Will try again later.");
            return false;
        }

        this.accessToken = resultJsonObject.optString("access_token");

        return !TextUtils.isEmpty(this.accessToken);
    }

}
