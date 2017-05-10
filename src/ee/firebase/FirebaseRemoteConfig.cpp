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

#include <firebase/remote_config.h>

namespace ee {
namespace firebase {
FirebaseRemoteConfig::FirebaseRemoteConfig() {
    initialized_ = false;
    defaultsDirty_ = false;
}

FirebaseRemoteConfig::~FirebaseRemoteConfig() {
    if (initialized_) {
        ::firebase::remote_config::Terminate();
    }
}

bool FirebaseRemoteConfig::initialize() {
    if (initialized_) {
        return true;
    }

    FirebaseApp::initialize();
    auto app = ::firebase::App::GetInstance();
    if (app == nullptr) {
        return false;
    }

    auto initResult = ::firebase::remote_config::Initialize(*app);
    if (initResult != ::firebase::InitResult::kInitResultSuccess) {
        return false;
    }

    fetchScheduler_ = std::make_unique<FirebaseScheduler<void>>();
    initialized_ = true;
    return true;
}

void FirebaseRemoteConfig::fetch(bool devModeEnabled,
                                 const FetchCallback& callback) {
    auto guard = std::make_shared<core::ScopeGuard>(std::bind(callback, false));
    if (not initialized_) {
        return;
    }
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
}

void FirebaseRemoteConfig::setDefaultBool(const std::string& key, bool value) {
    defaults_[key] = value;
    defaultsDirty_ = true;
}

void FirebaseRemoteConfig::setDefaultLong(const std::string& key,
                                          std::int64_t value) {
    defaults_[key] = value;
    defaultsDirty_ = true;
}

void FirebaseRemoteConfig::setDefaultDouble(const std::string& key,
                                            double value) {
    defaults_[key] = value;
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
    std::vector<::firebase::remote_config::ConfigKeyValueVariant> values;
    values.reserve(defaults_.size());
    std::copy(values.cbegin(), values.cend(), std::back_inserter(values));
    SetDefaults(std::addressof(values.at(0)), values.size());
    defaultsDirty_ = false;
}

bool FirebaseRemoteConfig::getBool(const std::string& key) {
    if (not initialized_) {
        return defaults_[key].bool_value();
    }
    ::firebase::remote_config::ValueInfo info;
    auto result = GetBoolean(key.c_str(), std::addressof(info));
    return result;
}

std::int64_t FirebaseRemoteConfig::getLong(const std::string& key) {
    if (not initialized_) {
        return defaults_[key].bool_value();
    }
    ::firebase::remote_config::ValueInfo info;
    auto result = GetLong(key.c_str(), std::addressof(info));
    return result;
}

double FirebaseRemoteConfig::getDouble(const std::string& key) {
    if (not initialized_) {
        return defaults_[key].double_value();
    }
    ::firebase::remote_config::ValueInfo info;
    auto result = GetDouble(key.c_str(), std::addressof(info));
    return result;
}

std::string FirebaseRemoteConfig::getString(const std::string& key) {
    if (not initialized_) {
        return defaults_[key].string_value();
    }
    ::firebase::remote_config::ValueInfo info;
    auto result = GetString(key.c_str(), std::addressof(info));
    return result;
}
} // namespace firebase
} // namespace ee
