//
//  EEPreferences.hpp
//  ee-library
//
//  Created by Zinge on 2/27/17.
//
//

#ifndef EE_LIBRARY_PREFERENCES_HPP_
#define EE_LIBRARY_PREFERENCES_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "EEMacro.hpp"

NS_EE_BEGIN
using DataStorage = std::unordered_map<std::string, std::string>;

class DataHandler;

class Preferences {
private:
    using Self = Preferences;

protected:
    enum class Encryption {
        /// No encryption, fastest saving/loading.
        None,

        /// Encrypts/decrypts depends on the value.
        Value,

        /// Encrypts/decrypts depends on the combination of key and value.
        KeyValue
    };

public:
    void initializeWithFilename(const std::string& filename);

    void forceLoadData();

protected:
    Preferences();
    ~Preferences();

    /// Invoked when ee::set is called.
    virtual void onSaving(std::size_t dataId, const std::string& key,
                          const std::string& value) = 0;

    /// Invoked when ee::get is called.
    virtual bool onLoading(std::size_t dataId, const std::string& key,
                           std::string& value) = 0;

    /// Gets the default value for the specified data id and key.
    virtual std::string getDefaultValue(std::size_t dataId,
                                        const std::string& key) const = 0;

    /// Saves the specified key and value to the persisted data.
    void save(const std::string& key, const std::string& value,
              Encryption mode);

    /// Loads the specified key from the persisted data.
    /// @return True if the specified key exists, false otherwise.
    bool load(const std::string& key, std::string& value, Encryption mode);

    /// Calculates a hash for the specified input.
    virtual std::string calculateHash(const std::string& input) const = 0;

    bool isInitialized() const { return isInitialized_; }

private:
    /// Encrypts ONLY the specified value.
    /// @return An encrypted value.
    std::string encrypt(const std::string& value) const;

    /// Encrypts the specified pair of key and value.
    /// @return An encrypted value.
    std::string encrypt(const std::string& key, const std::string& value) const;

    /// Attempts to decrypt the specified value.
    /// @return Whether the decryption was successful.
    bool decrypt(const std::string& encryptedValue, std::string& result) const;

    /// Attempts to decrypt the specified value using the specified key,
    /// @return Whether the decryption was successful.
    bool decrypt(const std::string& encryptedValue, const std::string& key,
                 std::string& result) const;

    /// Merges dirty data into persisted data.
    /// @return Whether the persisted data is updated.
    bool merge();

    /// Merges the specified data into persisted data.
    /// @return Whether the persisted data is updated.
    bool merge(DataStorage& data);

    void updateData();

    std::string filePath_;

    /// Is the preferences initialized?
    bool isInitialized_;

    /// Dirty (persisted) data that is not yet merged.
    DataStorage dirtyData_;

    DataStorage persistedData_;

    /// Stores the session data.
    DataStorage sessionData_;

    std::unique_ptr<DataHandler> handler_;
};
NS_EE_END

#endif /* EE_LIBRARY_PREFERENCES_HPP_ */
