//
//  KeyValueStorage.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_KEY_VALUE_STORAGE_HPP_
#define EE_X_STORE_KEY_VALUE_STORAGE_HPP_

#include <string>
#include <vector>

namespace ee {
namespace store {
class KeyValueStorage {
public:
    static KeyValueStorage& getInstance();

    /// Retrieves the value for the given key.
    /// @param key The key in the key-val pair.
    /// @return The value for the give nkey.
    virtual std::string getValue(const std::string& key) const = 0;

    /// Sets the given value to the given key.
    /// @param val The val in the key-val pair.
    /// @param key The key in the key-val pair.
    virtual void setValue(const std::string& key, const std::string& val) = 0;

    /// Gets all keys in the storage with no encryption.
    /// @return A vector of unencrypted keys.
    virtual std::vector<std::string> getEncryptedKeys() = 0;

    /// Deletes the key-val pair with the given key.
    /// @param key The key in the key-val pair.
    virtual void deleteKeyValue(const std::string& key) = 0;

    /// Purges the entire storage.
    virtual void purge() = 0;

protected:
    KeyValueStorage() = default;
    virtual ~KeyValueStorage() = default;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_KEY_VALUE_STORAGE_HPP_ */
