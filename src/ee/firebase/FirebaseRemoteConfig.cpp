//
//  FirebaseRemoteConfig.cpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#include "ee/firebase/FirebaseRemoteConfig.hpp"
#include "ee/firebase/FirebaseApp.hpp"
#include "ee/firebase/FirebaseScheduler.hpp"
#include "ee/core/ScopeGuard.hpp"

#ifndef __MACH__
#include <firebase/remote_config.h>
#endif // __MACH__

namespace ee {
namespace firebase {
FirebaseRemoteConfig::FirebaseRemoteConfig() {
    initialized_ = false;
    defaultsDirty_ = false;
}

FirebaseRemoteConfig::~FirebaseRemoteConfig() {
    if (initialized_) {
#ifndef __MACH__
        ::firebase::remote_config::Terminate();
#endif // __MACH__
    }
}

bool FirebaseRemoteConfig::initialize() {
    if (initialized_) {
        return true;
    }

    FirebaseApp::initialize();

#ifndef __MACH__
    auto app = ::firebase::App::GetInstance();
    if (app == nullptr) {
        return false;
    }

    auto initResult = ::firebase::remote_config::Initialize(*app);
    if (initResult != ::firebase::InitResult::kInitResultSuccess) {
        return false;
    }

    fetchScheduler_ = std::make_unique<FirebaseScheduler<void>>();
#endif // __MACH__

    initialized_ = true;
    return true;
}

void FirebaseRemoteConfig::fetch(bool devModeEnabled,
                                 const FetchCallback& callback) {
    auto guard = std::make_shared<core::ScopeGuard>(std::bind(callback, false));
    if (not initialized_) {
        return;
    }

#ifndef __MACH__
    SetConfigSetting(
        ::firebase::remote_config::ConfigSetting::kConfigSettingDeveloperMode,
        devModeEnabled ? "1" : "0");
    fetchScheduler_->push(
        ::firebase::remote_config::Fetch(),
        [callback, guard](const ::firebase::Future<void>& future) {
            auto fetched = ::firebase::remote_config::ActivateFetched();
            if (not fetched) {
                return;
            }
            guard->dismiss();
            callback(true);
        });
#endif // __MACH__
}

void FirebaseRemoteConfig::setDefaultBool(const std::string& key, bool value) {
#ifndef __MACH__
    defaults_[key] = value;
#else  // __MACH__
    defaults_[key] = (value ? "true" : "false");
#endif // __MACH__
    defaultsDirty_ = true;
}

void FirebaseRemoteConfig::setDefaultLong(const std::string& key,
                                          std::int64_t value) {
#ifndef __MACH__
    defaults_[key] = value;
#else  // __MACH__
    defaults_[key] = std::to_string(value);
#endif // __MACH__
    defaultsDirty_ = true;
}

void FirebaseRemoteConfig::setDefaultDouble(const std::string& key,
                                            double value) {
#ifndef __MACH__
    defaults_[key] = value;
#else  // __MACH__
    defaults_[key] = std::to_string(value);
#endif // __MACH__
    defaultsDirty_ = true;
}

void FirebaseRemoteConfig::setDefaultString(const std::string& key,
                                            const std::string& value) {
    defaults_[key] = value;
    defaultsDirty_ = true;
}

void FirebaseRemoteConfig::flushDefaults() {
    if (not initialized_) {
        return;
    }
    if (not defaultsDirty_) {
        return;
    }
#ifndef __MACH__
    std::vector<::firebase::remote_config::ConfigKeyValueVariant> values;
    values.reserve(defaults_.size());
    std::copy(values.cbegin(), values.cend(), std::back_inserter(values));
    SetDefaults(std::addressof(values.at(0)), values.size());
#endif // __MACH__
    defaultsDirty_ = false;
}

bool FirebaseRemoteConfig::getBool(const std::string& key) {
#ifndef __MACH__
    if (not initialized_) {
        return defaults_[key].bool_value();
    }
    ::firebase::remote_config::ValueInfo info;
    auto result = GetBoolean(key.c_str(), std::addressof(info));
    return result;
#else  // __MACH__
    return defaults_[key] == "true";
#endif // __MACH__
}

std::int64_t FirebaseRemoteConfig::getLong(const std::string& key) {
#ifndef __MACH__
    if (not initialized_) {
        return defaults_[key].bool_value();
    }
    ::firebase::remote_config::ValueInfo info;
    auto result = GetLong(key.c_str(), std::addressof(info));
    return result;
#else  // __MACH__
    return std::stoll(defaults_[key]);
#endif // __MACH__
}

double FirebaseRemoteConfig::getDouble(const std::string& key) {
#ifndef __MACH__
    if (not initialized_) {
        return defaults_[key].double_value();
    }
    ::firebase::remote_config::ValueInfo info;
    auto result = GetDouble(key.c_str(), std::addressof(info));
    return result;
#else  // __MACH__
    return std::stod(defaults_[key]);
#endif // __MACH__
}

std::string FirebaseRemoteConfig::getString(const std::string& key) {
#ifndef __MACH__
    if (not initialized_) {
        return defaults_[key].string_value();
    }
    ::firebase::remote_config::ValueInfo info;
    auto result = GetString(key.c_str(), std::addressof(info));
    return result;
#else  // __MACH__
    return defaults_[key];
#endif // __MACH__
}
} // namespace firebase
} // namespace ee
