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

#include "CCNativeCoreBridge.h"
#include "CCNdkBridge.h"
#include "CCSoomlaUtils.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <string>

#define CLASS_NAME "com/soomla/cocos2dx/common/CoreBridgeBinder"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "CCCoreBridgeBinderIos.h"

#endif

namespace soomla {

USING_NS_CC;

#define TAG "SOOMLA NativeCoreBridge"

CCNativeCoreBridge::CCNativeCoreBridge() {
    // Just bind to native before initing
    this->bindNative();
}

void CCNativeCoreBridge::bindNative() {
    CCSoomlaUtils::logDebug(TAG, "Binding to native platform bridge...");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;

    bool exists =
        JniHelper::getStaticMethodInfo(minfo, CLASS_NAME, "bind", "()V");

    if (exists) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    } else {
        CCSoomlaUtils::logError(TAG, "Unable to bind native bridge on Android");
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    soomla::CCCoreBridgeBinderIos::bind();
#endif
}
} // namespace soomla
