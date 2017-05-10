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

#ifndef __MACH__
namespace firebase {
class Variant;
} // namespace firebase
#endif // __MACH__

namespace ee {
namespace firebase {
#ifndef __MACH__
template <class T> class FirebaseScheduler;
#endif // __MACH__

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

#ifndef __MACH__
    std::map<std::string, ::firebase::Variant> defaults_;
    std::unique_ptr<FirebaseScheduler<void>> fetchScheduler_;
#else  // __MACH__
    std::map<std::string, std::string> defaults_;
#endif // __MACH__
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_REMOTE_CONFIG_HPP_ */
