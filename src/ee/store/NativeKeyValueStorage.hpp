//
//  NativeKeyValueStorage.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_NATIVE_KEY_VALUE_STORAGE_HPP -
#define EE_X_STORE_NATIVE_KEY_VALUE_STORAGE_HPP

#include "ee/store/KeyValueStorage.hpp"

namespace ee {
namespace store {
/// The native implementation of KeyValueStore, uses the bridge to talk to
/// native implementation of KeyValueStorage.
class NativeKeyValueStorage : public KeyValueStorage {
public:
    virtual std::string getValue(const std::string& key) const override;

    virtual void setValue(const std::string& key,
                          const std::string& val) override;

    virtual std::vector<std::string> getEncryptedKeys() override;

    virtual void deleteKeyValue(const std::string& key) override;

    virtual void purge() override;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_NATIVE_KEY_VALUE_STORAGE_HPP */
