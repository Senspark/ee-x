package com.soomla;

import android.app.Activity;

/**
 * Created by refaelos on 17/06/14.
 */
public class Soomla {

	public static void initialize(Activity mainActivity, String secret) {
		initialize(secret);
		SoomlaApp soomlaApp = new SoomlaApp();
		soomlaApp.setApplication(mainActivity.getApplication());
		soomlaApp.onCreate();
	}

	@Deprecated
    public static void initialize(String secret) {
        SECRET = secret;
    }

    public static String SECRET = "SOOMLA_SEC";

    public static final String VERSION = "1.3.0";
}
