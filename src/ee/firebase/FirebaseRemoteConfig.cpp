//
//  FirebaseRemoteConfig.cpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#include <cassert>

#include "ee/core/internal/ScopeGuard.hpp"
#include "ee/firebase/FirebaseApp.hpp"
#include "ee/firebase/FirebaseRemoteConfig.hpp"
#include "ee/firebase/FirebaseScheduler.hpp"

#if defined(EE_X_MOBILE)
#include <firebase/remote_config.h>
#endif // EE_X_MOBILE

namespace ee {
namespace firebase {
using Self = RemoteConfig;

Self::RemoteConfig() {
    initialized_ = false;
    defaultsDirty_ = false;

#if defined(EE_X_MOBILE)
    fetchScheduler_ = nullptr;
#endif // EE_X_MOBILE
}

Self::~RemoteConfig() {
    if (initialized_) {
#if defined(EE_X_MOBILE)
        ::firebase::remote_config::Terminate();
#endif // EE_X_MOBILE
    }
}

bool Self::initialize() {
    if (initialized_) {
        return true;
    }

    App::initialize();

#if defined(EE_X_MOBILE)
    auto app = ::firebase::App::GetInstance();
    if (app == nullptr) {
        return false;
    }

    auto initResult = ::firebase::remote_config::Initialize(*app);
    if (initResult != ::firebase::InitResult::kInitResultSuccess) {
        return false;
    }

    fetchScheduler_ = std::make_unique<Scheduler<void>>();
#endif // EE_X_MOBILE

    initialized_ = true;
    return true;
}

void Self::fetchJS(const FetchCallback& callback) {
    fetch(false, callback);
}

void Self::fetch(bool devModeEnabled, const FetchCallback& callback) {
    auto guard = std::make_shared<core::ScopeGuard>(std::bind(callback, false));
    if (not initialized_) {
        return;
    }

#if defined(EE_X_MOBILE)
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
            if (callback) {
                callback(true);
            }
        });
#endif // EE_X_MOBILE
}

void Self::setDefaultBool(const std::string& key, bool value) {
#if defined(EE_X_MOBILE)
    defaults_[key] = value;
#else  // EE_X_MOBILE
    defaults_[key] = (value ? "true" : "false");
#endif // EE_X_MOBILE
    defaultsDirty_ = true;
}

void Self::setDefaultLong(const std::string& key, std::int64_t value) {
#if defined(EE_X_MOBILE)
    defaults_[key] = value;
#else  // EE_X_MOBILE
    defaults_[key] = std::to_string(value);
#endif // EE_X_MOBILE
    defaultsDirty_ = true;
}

void Self::setDefaultDouble(const std::string& key, double value) {
#if defined(EE_X_MOBILE)
    defaults_[key] = value;
#else  // EE_X_MOBILE
    defaults_[key] = std::to_string(value);
#endif // EE_X_MOBILE
    defaultsDirty_ = true;
}

void Self::setDefaultString(const std::string& key, const std::string& value) {
    defaults_[key] = value;
    defaultsDirty_ = true;
}

void Self::flushDefaults() {
    if (not initialized_) {
        return;
    }
    if (not defaultsDirty_) {
        return;
    }
#if defined(EE_X_MOBILE)
    std::vector<::firebase::remote_config::ConfigKeyValueVariant> values;
    values.reserve(defaults_.size());
    for (auto&& elt : defaults_) {
        values.push_back({elt.first.c_str(), elt.second});
    }
    SetDefaults(std::addressof(values.at(0)), values.size());
#endif // EE_X_MOBILE
    defaultsDirty_ = false;
}

bool Self::getBool(const std::string& key) {
#if defined(EE_X_MOBILE)
    if (not initialized_) {
        return defaults_[key].bool_value();
    }
    if (defaultsDirty_) {
        // Please call flushDefaults().
        assert(false);
    }
    ::firebase::remote_config::ValueInfo info;
    auto result = GetBoolean(key.c_str(), std::addressof(info));
    return result;
#else  // EE_X_MOBILE
    return defaults_[key] == "true";
#endif // EE_X_MOBILE
}

std::int64_t Self::getLong(const std::string& key) {
#if defined(EE_X_MOBILE)
    if (not initialized_) {
        return defaults_[key].int64_value();
    }
    if (defaultsDirty_) {
        // Please call flushDefaults().
        assert(false);
    }
    ::firebase::remote_config::ValueInfo info;
    auto result = GetLong(key.c_str(), std::addressof(info));
    return result;
#else  // EE_X_MOBILE
    return std::stoll(defaults_[key]);
#endif // EE_X_MOBILE
}

double Self::getDouble(const std::string& key) {
#if defined(EE_X_MOBILE)
    if (not initialized_) {
        return defaults_[key].double_value();
    }
    if (defaultsDirty_) {
        // Please call flushDefaults().
        assert(false);
    }
    ::firebase::remote_config::ValueInfo info;
    auto result = GetDouble(key.c_str(), std::addressof(info));
    return result;
#else  // EE_X_MOBILE
    return std::stod(defaults_[key]);
#endif // EE_X_MOBILE
}

std::string Self::getString(const std::string& key) {
#if defined(EE_X_MOBILE)
    if (not initialized_) {
        return defaults_[key].string_value();
    }
    if (defaultsDirty_) {
        // Please call flushDefaults().
        assert(false);
    }
    ::firebase::remote_config::ValueInfo info;
    auto result = GetString(key.c_str(), std::addressof(info));
    return result;
#else  // EE_X_MOBILE
    return defaults_[key];
#endif // EE_X_MOBILE
}
} // namespace firebase
} // namespace ee
