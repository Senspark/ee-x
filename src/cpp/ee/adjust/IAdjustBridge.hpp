//
//  IAdjustBridge.hpp
//  Adjust
//
//  Created by eps on 8/19/20.
//

#ifndef EE_X_I_ADJUST_BRIDGE_HPP
#define EE_X_I_ADJUST_BRIDGE_HPP

#ifdef __cplusplus

#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/adjust/AdjustFwd.hpp"

namespace ee {
namespace adjust {
class IBridge : public IPlugin {
public:
    virtual void initialize(const AdjustConfig& config) = 0;

    /// Enables/disables the Adjust SDK from tracking any activities of the
    /// current device. This setting is remembered between sessions.
    virtual void setEnabled(bool enabled) = 0;

    /// Gets iOS advertising identifier or Google Play Services advertising
    /// identifier
    virtual Task<std::string> getAdvertisingIdentifier() const = 0;

    /// Gets Adjust device identifier.
    /// Only available after the app's installation has been tracked by the
    /// Adjust backend.
    virtual std::string getDeviceIdentifier() const = 0;

    /// To send us a push notification token, add the following call to Adjust
    /// whenever your app receives the token or it is updated.
    virtual void setPushToken(const std::string& token) = 0;

    virtual void trackEvent(const std::string& token) = 0;
};
} // namespace adjust
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_ADJUST_BRIDGE_HPP */
