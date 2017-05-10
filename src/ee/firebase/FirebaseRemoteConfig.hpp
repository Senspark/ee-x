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

namespace firebase {
class Variant;
} // namespace firebase

namespace ee {
namespace firebase {
template <class T> class FirebaseScheduler;

class FirebaseRemoteConfig {
public:
    using FetchCallback = std::function<void(bool succeeded)>;

    FirebaseRemoteConfig();
    ~FirebaseRemoteConfig();

    bool initialize();

    void fetch(bool devModeEnabled, const FetchCallback& callback);

    void setDefaultBool(const std::string& key, bool value);
    void setDefaultLong(const std::string& key, std::int64_t value);
    void setDefaultDouble(const std::string& key, double value);
    void setDefaultString(const std::string& key, const std::string& value);

    void flushDefaults();

    bool getBool(const std::string& key);
    std::int64_t getLong(const std::string& key);
    double getDouble(const std::string& key);
    std::string getString(const std::string& key);

private:
    bool initialized_;

    bool defaultsDirty_;
    std::map<std::string, ::firebase::Variant> defaults_;

    std::unique_ptr<FirebaseScheduler<void>> fetchScheduler_;
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_REMOTE_CONFIG_HPP_ */
