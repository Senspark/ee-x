//
//  MessageBridgeSwift.h
//  Pods
//
//  Created by eps on 6/7/20.
//

#ifndef EE_X_MESSAGE_BRIDGE_SWIFT_H
#define EE_X_MESSAGE_BRIDGE_SWIFT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern const char* ee_callCppInternal(const char* tag, const char* message);
extern void ee_freeString(const char* str);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* EE_X_MESSAGE_BRIDGE_SWIFT_H */
