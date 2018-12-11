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
#include "CCBridgelessKeyValueStorage.h"

#if __has_include(<base/CCDirector.h>)
#define COCOSCREATOR_VERSION 1
#else
#define COCOSCREATOR_VERSION 2
#endif

#if COCOSCREATOR_VERSION == 2
#include "CCNativeKeyValueStorage.h"
#elif COCOSCREATOR_VERSION == 1
#include "base/CCUserDefault.h"
#endif

namespace soomla {

USING_NS_CC;

#define KEY_VALUE_STORAGE_KEY "soomla.kvs.keys"

CCBridgelessKeyValueStorage::CCBridgelessKeyValueStorage() {
    loadStoredKeys();
}

std::string CCBridgelessKeyValueStorage::getValue(const std::string& key) const {
    std::string defaultValue = "";
    std::string result =
#if COCOSCREATOR_VERSION == 2
        CCKeyValueStorage::getInstance()->getValue(key.c_str());
#elif COCOSCREATOR_VERSION == 1
        UserDefault::getInstance()->getStringForKey(key.c_str(), defaultValue);
#endif

    if (result == defaultValue) {
        return "";
    }

    return result;
}

void CCBridgelessKeyValueStorage::setValue(const std::string &key, const std::string &val) {
    
#if COCOSCREATOR_VERSION == 2
    CCKeyValueStorage::getInstance()->setValue(key.c_str(), val);
#elif COCOSCREATOR_VERSION == 1
    UserDefault::getInstance()->setStringForKey(key.c_str(), val);
    UserDefault::getInstance()->flush();
#endif
    addStoredKeys(key);
    saveStoredKeys();
}

std::vector<std::string> CCBridgelessKeyValueStorage::getEncryptedKeys() const {
    std::vector<std::string> encryptedKeys;
    for (auto& key: mStoredKeys) {
        encryptedKeys.push_back(key);
    }
    return encryptedKeys;
}

void CCBridgelessKeyValueStorage::purge() {
    for (auto key : mStoredKeys) {
        deleteKeyValue(key);
    }

    mStoredKeys.clear();
    
#if COCOSCREATOR_VERSION == 2
    CCKeyValueStorage::getInstance()->setValue(KEY_VALUE_STORAGE_KEY, "");
#elif COCOSCREATOR_VERSION == 1
    UserDefault::getInstance()->setStringForKey(KEY_VALUE_STORAGE_KEY, "");
    UserDefault::getInstance()->flush();
#endif
}

void CCBridgelessKeyValueStorage::addStoredKeys(const std::string& key) {
    if (mStoredKeys.count(key)) {
        return;
    } else {
        mStoredKeys.emplace(key);
    }
}

void CCBridgelessKeyValueStorage::removeStoredKeys(const std::string& key) {
    mStoredKeys.erase(key);
}

void CCBridgelessKeyValueStorage::saveStoredKeys() {
    std::string joinedKeys = "";
    for (auto key : mStoredKeys) {
        joinedKeys.append("#").append(key);
    }
    
#if COCOSCREATOR_VERSION == 2
    CCKeyValueStorage::getInstance()->setValue(KEY_VALUE_STORAGE_KEY, joinedKeys);
#elif COCOSCREATOR_VERSION == 1
    UserDefault::getInstance()->setStringForKey(KEY_VALUE_STORAGE_KEY, joinedKeys);
    UserDefault::getInstance()->flush();
#endif
}

void CCBridgelessKeyValueStorage::loadStoredKeys() {
    std::string joinedKeys =
#if COCOSCREATOR_VERSION == 2
    CCKeyValueStorage::getInstance()->getValue(KEY_VALUE_STORAGE_KEY);
#elif COCOSCREATOR_VERSION == 1
    UserDefault::getInstance()->getStringForKey(KEY_VALUE_STORAGE_KEY, "");
#endif

    std::stringstream ss(joinedKeys);
    std::string item;
    while (std::getline(ss, item, '#')) {
        addStoredKeys(item.c_str());
    }
}
} // namespace soomla
