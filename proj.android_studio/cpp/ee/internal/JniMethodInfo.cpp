//
//  EEJniMethodInfo.cpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#include "ee/internal/JniMethodInfo.hpp"

namespace ee {
namespace core {
std::unique_ptr<JniMethodInfo> JniMethodInfo::create(JNIEnv* env, jclass clazz,
                                                     jmethodID methodId) {
    return std::unique_ptr<JniMethodInfo>(
        new JniMethodInfo(env, clazz, methodId));
}

JniMethodInfo::JniMethodInfo(JNIEnv* env, jclass clazz, jmethodID methodId)
    : env_{env}
    , clazz_{clazz}
    , methodId_{methodId} {
}

JniMethodInfo::~JniMethodInfo() {
    if (env_ != nullptr && clazz_ != nullptr) {
        // Release resources.
        env_->DeleteLocalRef(clazz_);
    }
}

jclass JniMethodInfo::getClass() const noexcept {
    return clazz_;
}

jmethodID JniMethodInfo::getMethodId() const noexcept {
    return methodId_;
}

JNIEnv* JniMethodInfo::getEnv() const noexcept {
    return env_;
}
} // namespace core
} // namespace ee
