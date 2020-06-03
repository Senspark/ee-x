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

#ifndef __CCKeyValueStorage_H
#define __CCKeyValueStorage_H

#ifdef __cplusplus

#include <cocos2d.h>

#define DB_KEY_PRFIX "soomla."

namespace soomla {

/**
 Provides an API to the Soomla's native key-value storage

 All persistent information regarding the user's usage of the framework's
 various capabilities is kept in a native storage on the user's device.
 This class provides a way to access the raw information and manipulate it.
 */
class CCKeyValueStorage : cocos2d::Ref {
public:
    /**
     Retrieves the singleton instance of the storage

     @return The singleton instance of the storage
     */
    static CCKeyValueStorage* getInstance();

    /**
     Retrieves the value for the given `key`.

     @param key The key in the key-val pair.
     @return The value for given key.
     */
    virtual std::string getValue(const std::string& key) const = 0;

    /**
     Sets the given value to the given `key`.

     @param val The val in the key-val pair.
     @param key The key in the key-val pair.
     */
    virtual void setValue(const std::string& key, const std::string& val) = 0;

    /**
     * Gets all keys in the storage with no encryption
     *
     * @return a List of unencrypted keys
     */
    virtual std::vector<std::string> getEncryptedKeys() const = 0;
    /**
     Deletes the key-val pair with the given `key`.

     @param key The key in the key-val pair.
     */
    virtual void deleteKeyValue(const std::string& key) = 0;

    /**
     Purges the entire storage

     NOTE: Use with caution, since this will delete all the user's local
     information This message is used mainly for while in testing.
     */
    virtual void purge() = 0;
};
} // namespace soomla

#endif // __cplusplus

#endif // __CCKeyValueStorage_H
