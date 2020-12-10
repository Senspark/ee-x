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

package com.soomla;

import android.app.Application;
import android.content.Context;
import android.os.Build;

import com.soomla.events.AppToBackgroundEvent;

public class SoomlaApp extends Application{

	protected static SoomlaApp mInstance = null;
	protected static Application mAppInstance = null;

	@Deprecated
	public static SoomlaApp instance() {
    	return mInstance;
    }

	public static Application appInstance() {
		return mAppInstance;
	}

    private Thread.UncaughtExceptionHandler defaultUEH;

    public static Foreground ForegroundService;

    @Override
    public void onCreate() {
        super.onCreate();
        mInstance = this;
		if (mAppInstance == null) {
			mAppInstance = this;
		}
        context = getApplicationContext();

        // Fixing AsyncTask not found exception
        try {
            Class.forName("android.os.AsyncTask");
        }
        catch(Throwable ignore) {
            // ignored
        }

        ForegroundService = Foreground.init();

        defaultUEH = Thread.getDefaultUncaughtExceptionHandler();
        Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(Thread thread, Throwable e) {
                BusProvider.getInstance().post(new AppToBackgroundEvent());
                defaultUEH.uncaughtException(thread, e);
            }
        });
    }

	@Override
	public Context getApplicationContext() {
		try {
			return super.getApplicationContext();
		} catch (NullPointerException e) {
			return context;
		}
	}

	/** Setters and Getters */

	@Deprecated
    public static void setExternalContext(Context oContext) {
        context = oContext;
    }

	public static void setApplication(Application application) {
		mAppInstance = application;
		context = application.getApplicationContext();
	}

    public static Context getAppContext() {
        return context;
    }


    /** Private Members */

    private static Context context;


}
