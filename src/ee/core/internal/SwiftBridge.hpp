#ifndef EE_X_SWIFT_BRIDGE_HPP
#define EE_X_SWIFT_BRIDGE_HPP

/// Required for Swift.

#include "ee/Macro.hpp"

#if defined(EE_X_IOS) || defined(EE_X_OSX)
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/// Used in MessageBridge.
extern char* ee_callCppInternal(const char* tag, const char* message);

/// Used in Thread.
extern void ee_runOnMainThreadCallback();
extern void ee_runOnMainThreadDelayedCallback(int key);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)

#endif // EE_X_SWIFT_BRIDGE_HPP
