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

#include "ee/core/internal/JniExceptionChecker.hpp"

namespace ee {
namespace core {
/// RAII class for encapsulating jclass and jmethodID.
class JniMethodInfo final {
private:
    using Self = JniMethodInfo;

public:
    static std::unique_ptr<Self> create(JNIEnv* env, jclass clazz,
                                        jmethodID methodId);

    ~JniMethodInfo();

    template <class... Args>
    void callStaticVoidMethod(Args&&... args) {
        JniExceptionChecker checker(env_);
        return env_->CallStaticVoidMethod(clazz_, methodId_,
                                          std::forward<Args>(args)...);
    }

    template <class... Args>
    jboolean callStaticBooleanMethod(Args&&... args) {
        JniExceptionChecker checker(env_);
        return env_->CallStaticBooleanMethod(clazz_, methodId_,
                                             std::forward<Args>(args)...);
    }

    template <class... Args>
    jobject callStaticObjectMethod(Args&&... args) {
        JniExceptionChecker checker(env_);
        return env_->CallStaticObjectMethod(clazz_, methodId_,
                                            std::forward<Args>(args)...);
    }

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
