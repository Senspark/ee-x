//
//  FirebaseRemoteConfig.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FIREBASE_REMOTE_CONFIG_HPP_
#define EE_X_FIREBASE_REMOTE_CONFIG_HPP_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "ee/Macro.hpp"

#if defined(EE_X_MOBILE)
namespace firebase {
class Variant;
} // namespace firebase
#endif // EE_X_MOBILE

namespace ee {
namespace firebase {
#if defined(EE_X_MOBILE)
template <class T> class FirebaseScheduler;
#endif // EE_X_MOBILE

/// Wrapper for Firebase Remote Config.
/// Should have a single instance at a time.
/// https://firebase.google.com/docs/remote-config/use-config-cpp
class FirebaseRemoteConfig {
public:
    using FetchCallback = std::function<void(bool succeeded)>;

    FirebaseRemoteConfig();
    ~FirebaseRemoteConfig();

    /// Attempts to initialize the remote config.
    /// @return True if successfully initialized, false otherwise.
    bool initialize();

    void fetch(bool devModeEnabled, const FetchCallback& callback);

    /// Sets a default bool value for the given key.
    void setDefaultBool(const std::string& key, bool value);

    /// Sets a default long value for the given key.
    void setDefaultLong(const std::string& key, std::int64_t value);

    /// Sets a default double value for the given key.
    void setDefaultDouble(const std::string& key, double value);

    /// Sets a default string value for the given key.
    void setDefaultString(const std::string& key, const std::string& value);

    /// Flushes the default values.
    /// Call this after setting all default values.
    void flushDefaults();

    /// Gets a bool value for the given key.
    bool getBool(const std::string& key);

    /// Gets a long value for the given key.
    std::int64_t getLong(const std::string& key);

    /// Gets a double value for the given key.
    double getDouble(const std::string& key);

    /// Gets a string value for the given key.
    std::string getString(const std::string& key);

private:
    bool initialized_;
    bool defaultsDirty_;

#if defined(EE_X_MOBILE)
    std::map<std::string, ::firebase::Variant> defaults_;
    std::unique_ptr<FirebaseScheduler<void>> fetchScheduler_;
#else  // EE_X_MOBILE
    std::map<std::string, std::string> defaults_;
#endif // EE_X_MOBILE
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_REMOTE_CONFIG_HPP_ */
