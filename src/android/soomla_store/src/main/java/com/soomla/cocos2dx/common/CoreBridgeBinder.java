/*
 * Copyright (C) 2012-2014 Soomla Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

package com.soomla.cocos2dx.common;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;

import com.soomla.SoomlaUtils;

import java.lang.reflect.Method;

public class CoreBridgeBinder {

    public static void bind() {
        SoomlaUtils.LogDebug(TAG, "Binding to native bridge");

        try {
            Class<?> cocos2dxActivityClass = Class.forName("org.cocos2dx.lib.Cocos2dxActivity");
            Method getContextMethod = cocos2dxActivityClass.getMethod("getContext");
            Context context = (Context) getContextMethod.invoke(null);
            if (context instanceof Activity) {
                Activity cocos2dxActivity = (Activity) context;
                NdkGlue.getInstance().setActivity(cocos2dxActivity);
            }
            else {
                String message = "Unable to bind Core Bridge in native: cannot get main activity";
                SoomlaUtils.LogError(TAG, message);
                throw new IllegalStateException(message);
            }
            SoomlaUtils.LogDebug(TAG, "Main activity registered");

            Class<?> cocos2dxGlSurfaceClass = Class.forName("org.cocos2dx.lib.Cocos2dxGLSurfaceView");
            Method getInstanceMethod = cocos2dxGlSurfaceClass.getMethod("getInstance");
            GLSurfaceView surfaceView = (GLSurfaceView) getInstanceMethod.invoke(null);
            NdkGlue.getInstance().setGlSurfaceView(surfaceView);
            SoomlaUtils.LogDebug(TAG, "GLSurfaceView registered");
        } catch (Exception e) {
            String message = "Unable to bind Core Bridge in native: " + e.getLocalizedMessage();
            SoomlaUtils.LogError(TAG, message);
            throw new IllegalStateException(message);
        }

        CoreBridge.getInstance().init();
    }

    private static final String TAG = "SOOMLA CoreBridgeBinder";
}
