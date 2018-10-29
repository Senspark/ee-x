/*
 Copyright (C) 2012-2014 Soomla Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "CCNativeStoreBridge.h"
#include "CCNdkBridge.h"
#include "CCSoomlaUtils.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <string>

#define CLASS_NAME "com/soomla/cocos2dx/store/StoreBridgeBinder"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "CCStoreBridgeBinderIos.h"

#endif

namespace soomla {

USING_NS_CC;

#define TAG "SOOMLA CCNativeStoreBridge"

CCNativeStoreBridge::CCNativeStoreBridge() {
    // Just bind to native before initing
    this->bindNative();
}

void CCNativeStoreBridge::applyParams(const cocos2d::ValueMap& storeParams) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto SSV = false;
    if (storeParams.count("SSV")) {
        SSV = storeParams.at("SSV").asBool();
    }

    {
        ValueMap params;
        params["method"] = "CCSoomlaStore::setSSV";
        params["ssv"] = SSV;
        if (SSV) {
            auto verifyOnServerFailure = false;
            if (storeParams.count("verifyOnServerFailure")) {
                verifyOnServerFailure =
                    storeParams.at("verifyOnServerFailure").asBool();
            }
            params["verifyOnServerFailure"] = verifyOnServerFailure;
        }
        CCNdkBridge::callNative(params, nullptr);
    }
#endif

    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        {
            if (storeParams.count("androidPublicKey")) {
                auto&& androidPublicKey = storeParams.at("androidPublicKey").asString();
                ValueMap params;
                params["method"] = "CCSoomlaStore::setAndroidPublicKey";
                params["androidPublicKey"] = androidPublicKey;
                CCNdkBridge::callNative(params, nullptr);
            }
        }

        {
            if (storeParams.count("clientId") &&
                storeParams.count("clientSecret") &&
                storeParams.count("refreshToken") &&
                storeParams.count("verifyOnServerFailure")) {
                
                auto&& clientId = storeParams.at("clientId").asString();
                auto&& clientSecret = storeParams.at("clientSecret").asString();
                auto&& refreshToken = storeParams.at("refreshToken").asString();
                auto verifyOnServerFailure = storeParams.at("verifyOnServerFailure").asBool();
                
                ValueMap params;
                params["method"] = "CCSoomlaStore::configVerifyPurchases";
                params["clientId"] = clientId;
                params["clientSecret"] = clientSecret;
                params["refreshToken"] = refreshToken;
                params["verifyOnServerFailure"] = verifyOnServerFailure;
                CCNdkBridge::callNative(params, nullptr);
            }
        }

        {
            auto testPurchases = false;
            if (storeParams.count("testPurchases")) {
                testPurchases = storeParams.at("testPurchases").asBool();
            }
            
            ValueMap params;
            params["method"] = "CCSoomlaStore::setTestPurchases";
            params["testPurchases"] = testPurchases;
            CCNdkBridge::callNative(params, nullptr);
        }
#endif
    }
}

void CCNativeStoreBridge::bindNative() {
    CCSoomlaUtils::logDebug(TAG, "Binding to native platform Store bridge...");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;

    bool exists =
        JniHelper::getStaticMethodInfo(minfo, CLASS_NAME, "bind", "()V");

    if (exists) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    } else {
        CCSoomlaUtils::logError(
            TAG, "Unable to bind native Store bridge on Android");
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    soomla::CCStoreBridgeBinderIos::bind();
#endif
}
} // namespace soomla
