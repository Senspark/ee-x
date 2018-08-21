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

#include "base/CCUserDefault.h"

#include "CCBridgelessKeyValueStorage.h"

namespace soomla {

USING_NS_CC;

#define KEY_VALUE_STORAGE_KEY "soomla.kvs.keys"

CCBridgelessKeyValueStorage::CCBridgelessKeyValueStorage() {
    loadStoredKeys();
}

std::string
CCBridgelessKeyValueStorage::getValue(const std::string& key) const {
    std::string defaultValue = "";
    std::string result =
        UserDefault::getInstance()->getStringForKey(key.c_str(), defaultValue);
    if (result == defaultValue) {
        return "";
    }

    return result;
}

void CCBridgelessKeyValueStorage::setValue(const std::string &key, const std::string &val) {
    UserDefault::getInstance()->setStringForKey(key.c_str(), val);
    UserDefault::getInstance()->flush();
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

    UserDefault::getInstance()->setStringForKey(KEY_VALUE_STORAGE_KEY, "");
    UserDefault::getInstance()->flush();
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
    
    UserDefault::getInstance()->setStringForKey(KEY_VALUE_STORAGE_KEY,
                                                joinedKeys);
    UserDefault::getInstance()->flush();
}

void CCBridgelessKeyValueStorage::loadStoredKeys() {
    std::string joinedKeys =
        UserDefault::getInstance()->getStringForKey(KEY_VALUE_STORAGE_KEY, "");

    std::stringstream ss(joinedKeys);
    std::string item;
    while (std::getline(ss, item, '#')) {
        addStoredKeys(item.c_str());
    }
}
} // namespace soomla
