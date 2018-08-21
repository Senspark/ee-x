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

#include "CCJsonHelper.h"
#include "CCNdkBridge.h"
#include "CCSoomlaUtils.h"

USING_NS_CC;

#define LOG_JSON 0

#if (LOG_JSON == 1)
#define TAG "JSON"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "CCSoomlaEventDispatcher.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <string>

#define CLASS_NAME "com/soomla/cocos2dx/common/NdkGlue"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import "CCNdkBridgeIos.h"

#endif

namespace soomla {
extern "C" {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// Method for receiving NDK messages
void Java_com_soomla_cocos2dx_common_NdkGlue_sendCppMessage(JNIEnv* env,
                                                            jobject thiz,
                                                            jstring json) {
    std::string jsonString = JniHelper::jstring2string(json);
    const char* jsonCharArray = jsonString.c_str();

    json_error_t error;
    json_t* root;
    root = json_loads(jsonCharArray, 0, &error);

    if (!root) {
        fprintf(stderr, "error: at line #%d: %s\n", error.line, error.text);
        return;
    }

    cocos2d::Ref* dataToPass = CCJsonHelper::getCCObjectFromJson(root);

    CCSoomlaEventDispatcher::getInstance()->ndkCallback(
        (cocos2d::__Dictionary*)dataToPass);

    json_decref(root);
}
#endif

cocos2d::ValueMap CCNdkBridge::callNative(const cocos2d::ValueMap& params,
                                          soomla::CCError** pError) {
    auto&& methodParams = Value(params);

    json_t* toBeSentJson = CCJsonHelper::getJsonFromValue(methodParams);
    json_t* retJsonParams = NULL;

#if (LOG_JSON == 1)
    CCSoomlaUtils::logDebug(
        "CCNdkBridge",
        StringgUtils::format(
            "to native JSON: %s",
            json_dumps(toBeSentJson, JSON_COMPACT | JSON_ENSURE_ASCII))
            ->c_str());
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;

    if (JniHelper::getStaticMethodInfo(
            t, CLASS_NAME, "receiveCppMessage",
            "(Ljava/lang/String;)Ljava/lang/String;")) {
        char* jsonStrLocal =
            json_dumps(toBeSentJson, JSON_COMPACT | JSON_ENSURE_ASCII);
        std::string jsonStr(jsonStrLocal);
        free(jsonStrLocal);

        jstring stringArg1 = t.env->NewStringUTF(jsonStr.c_str());
        jstring retString = (jstring)t.env->CallStaticObjectMethod(
            t.classID, t.methodID, stringArg1);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);

        const char* nativeString = t.env->GetStringUTFChars(retString, 0);
        std::string retParamsStr(nativeString);
        t.env->ReleaseStringUTFChars(retString, nativeString);
        t.env->DeleteLocalRef(retString);

        const char* jsonCharArray = retParamsStr.c_str();

        json_error_t error;
        retJsonParams = json_loads(jsonCharArray, 0, &error);

        if (!retJsonParams) {
            fprintf(stderr, "error: at line #%d: %s\n", error.line, error.text);
            return ValueMapNull;
        }
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    retJsonParams = soomla::CCNdkBridgeIos::receiveCppMessage(toBeSentJson);
#endif

#if (LOG_JSON == 1)
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format(
                 "from native JSON: %s",
                 json_dumps(retJsonParams, JSON_COMPACT | JSON_ENSURE_ASCII))
                 .c_str());
#endif

    json_decref(toBeSentJson);
    auto&& retParams = CCJsonHelper::getValueFromJson(retJsonParams);

    if (retJsonParams) {
        json_decref(retJsonParams);
    }

    CCError* error = CCError::createWithValue(retParams);
    if (error != NULL) {
        if (pError != NULL) {
            *pError = error;
        } else {
            CCSoomlaUtils::logException("SOOMLA NdkBridge", error);
        }
    }

    return retParams.asValueMap();
}

}
} // namespace soomla
