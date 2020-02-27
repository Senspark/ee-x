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

#ifndef __CCBridgelessKeyValueStorage_H
#define __CCBridgelessKeyValueStorage_H

#include <set>

#include "soomla/data/CCKeyValueStorage.h"

namespace soomla {

/**
 The bridgeless implementation of CCKeyValueStorage, uses Cocos2dx
 facilities to store key-value data.

 See parent for all functions.
 */
class CCBridgelessKeyValueStorage : public CCKeyValueStorage {
public:
    CCBridgelessKeyValueStorage();
    virtual std::string getValue(const std::string& key) const override;

    virtual void setValue(const std::string& key,
                          const std::string& val) override;
    virtual std::vector<std::string> getEncryptedKeys() const override;
    virtual void deleteKeyValue(const std::string& key) override;
    virtual void purge() override;

protected:
    std::set<std::string> mStoredKeys;
    void addStoredKeys(const std::string& key);
    void removeStoredKeys(const std::string& key);
    void saveStoredKeys();
    void loadStoredKeys();
};
} // namespace soomla

#endif // __CCBridgelessKeyValueStorage_H
