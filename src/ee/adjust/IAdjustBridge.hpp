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

#include "ee/AdjustFwd.hpp"

namespace ee {
namespace adjust {
class IBridge : public IPlugin {
public:
    virtual void initialize(const AdjustConfig& config) = 0;
    virtual void setEnabled(bool enabled) = 0;
    virtual void trackEvent(const std::string& eventName) = 0;
};
} // namespace adjust
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_ADJUST_BRIDGE_HPP */
