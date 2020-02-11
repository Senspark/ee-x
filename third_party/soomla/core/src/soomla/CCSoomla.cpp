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

#include "soomla/CCSoomla.h"

#include "soomla/CCCoreBridge.h"
#include "soomla/CCNdkBridge.h"
#include "soomla/CCSoomlaUtils.h"

namespace soomla {

USING_NS_CC;

#define TAG "SOOMLA Soomla"

void CCSoomla::initialize(const std::string& soomlaSecret) {
    if (soomlaSecret.length() == 0) {
        CCSoomlaUtils::logError(TAG,
                                "Can't initialize SOOMLA without soomlaSecret");
        CCASSERT(true, "");
        return;
    }

    CCCoreBridge::initShared();

    ValueMap params;
    params["method"] = "CCSoomla::initialize";
    params["soomlaSecret"] = soomlaSecret;
    CCNdkBridge::callNative(params, nullptr);
}
} // namespace soomla
