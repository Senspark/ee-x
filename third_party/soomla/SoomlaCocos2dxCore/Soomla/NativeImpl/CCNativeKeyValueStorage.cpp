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

#include "CCError.h"
#include "CCNativeKeyValueStorage.h"
#include "CCNdkBridge.h"
#include "CCSoomlaUtils.h"

namespace soomla {

#define TAG "SOOMLA NativeKeyValueStorage"

USING_NS_CC;

std::string CCNativeKeyValueStorage::getValue(const std::string& key) const {
    CCError* error = NULL;

    ValueMap params;
    params["method"] = "CCNativeKeyValueStorage::getValue";
    params["key"] = key;

    auto retParams = CCNdkBridge::callNative(params, &error);

    if (error) {
        CCSoomlaUtils::logException(TAG, error);
        CC_ASSERT(false);
        return NULL;
    }

    if (retParams == ValueMapNull) {
        return NULL;
    }

    if (not retParams.count("return")) {
        return nullptr;
    }

    return retParams.at("return").asString();
}

void CCNativeKeyValueStorage::setValue(const std::string& key,
                                       const std::string& val) {
    CCError* error = NULL;

    ValueMap params;
    params["method"] = "CCNativeKeyValueStorage::setValue";
    params["key"] = key;
    params["val"] = val;

    CCNdkBridge::callNative(params, &error);

    if (error) {
        CCSoomlaUtils::logException(TAG, error);
        CC_ASSERT(false);
    }
}

void CCNativeKeyValueStorage::deleteKeyValue(const std::string &key) {
    CCError* error = NULL;

    ValueMap params;
    params["method"] = "CCNativeKeyValueStorage::deleteKeyValue";
    params["key"] = key;
    CCNdkBridge::callNative(params, &error);

    if (error) {
        CCSoomlaUtils::logException(TAG, error);
        CC_ASSERT(false);
    }
}

void CCNativeKeyValueStorage::purge() {
    CCError* error = NULL;
    ValueMap params;
    params["method"] = "CCNativeKeyValueStorage::purge";
    CCNdkBridge::callNative(params, &error);

    if (error) {
        CCSoomlaUtils::logException(TAG, error);
        CC_ASSERT(false);
    }
}

std::vector<std::string> CCNativeKeyValueStorage::getEncryptedKeys() const {
    CCError* error = NULL;
    std::vector<std::string> encryptedKeys;
    
    ValueMap params;
    params["method"] = "CCNativeKeyValueStorage::getEncryptedKeys";
    
    ValueMap&& retParams = CCNdkBridge::callNative(params, &error);

    if (error || retParams.size() == 0 || not retParams.count("return")) {
        CCSoomlaUtils::logException(TAG, error);
        CC_ASSERT(false);
        return encryptedKeys;
    }
    
    auto& retValue = retParams.at("return").asValueVector();
    for (auto value : retValue) {
        encryptedKeys.push_back(value.asString());
    }
    
    return encryptedKeys;
}

} // namespace soomla
