//
//  EEJniMethodInfo.hpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#ifndef EE_X_JNI_METHOD_INFO_HPP
#define EE_X_JNI_METHOD_INFO_HPP

#include <memory>

#include <jni.h>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
/// RAII class for encapsulating jclass and jmethodID.
class JniMethodInfo final {
public:
    static std::unique_ptr<JniMethodInfo> create(JNIEnv* env, jclass clazz,
                                                 jmethodID methodId);

    ~JniMethodInfo();

    /// Gets the pointer to the jclass.
    jclass getClass() const noexcept;

    /// Gets the method id.
    jmethodID getMethodId() const noexcept;

    /// Gets the pointer to the JNIEnv.
    JNIEnv* getEnv() const noexcept;

private:
    explicit JniMethodInfo(JNIEnv* env, jclass clazz, jmethodID methodId);

    /// Weak pointer to the JNIEnv.
    JNIEnv* env_;

    /// Strong pointer to the jclass.
    jclass clazz_;

    /// The method id.
    jmethodID methodId_;
};
} // namespace core
} // namespace ee

#endif /* EE_X_JNI_METHOD_INFO_HPP */
