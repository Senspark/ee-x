//
//  FirebaseRemoteConfig.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FIREBASE_REMOTE_CONFIG_BRIDGE_HPP
#define EE_X_FIREBASE_REMOTE_CONFIG_BRIDGE_HPP

#ifdef __cplusplus

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include <ee/FirebaseFwd.hpp>

namespace firebase {
class Variant;
} // namespace firebase

namespace ee {
namespace firebase {
namespace remote_config {
/// Describes the most recent fetch request status.
enum class LastFetchStatus {
    /// The most recent fetch was a success, and its data is ready to be
    /// applied, if you have not already done so.
    Success,

    /// The most recent fetch request failed.
    Failure,

    /// The most recent fetch is still in progress.
    Pending,
};

/// Describes the most recent fetch failure.
enum class FetchFailureReason {
    /// The fetch has not yet failed.
    Invalid,

    /// The most recent fetch failed because it was throttled by the server.
    /// (You are sending too many fetch requests in too short a time.)
    Throttled,

    /// The most recent fetch failed for an unknown reason.
    Error,
};

struct ConfigInfo {
    std::uint64_t fetchTime;
    LastFetchStatus lastFetchStatus;
    FetchFailureReason lastFetchFailureReason;
    std::uint64_t throttledEndTime;
};

/// Wrapper for Firebase Remote Config.
/// Should have a single instance at a time.
/// https://firebase.google.com/docs/remote-config/use-config-cpp
/// https://firebase.google.com/docs/reference/cpp/namespace/firebase/remote-config
class Bridge {
public:
    using FetchCallback = std::function<void(bool succeeded)>;

    Bridge();
    ~Bridge();

    /// Attempts to initialize the remote config.
    /// @return True if successfully initialized, false otherwise.
    bool initialize();

    /// Applies the most recently fetched data, so that its value can be
    /// accessed.
    bool activateFetched();

    /// Only fetches (activate fetch must be called manually).
    void fetchOnly(const std::function<void()>& callback);

    void fetch(bool devModeEnabled, const FetchCallback& callback);

    /// Returns information about the last fetch request, in the form of a
    /// ConfigInfo struct.
    ConfigInfo getInfo() const;

    /// Used for JSB.
    std::string getInfoJsb() const;

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

    std::map<std::string, ::firebase::Variant> defaults_;
};
} // namespace remote_config
} // namespace firebase
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_FIREBASE_REMOTE_CONFIG_BRIDGE_HPP */
