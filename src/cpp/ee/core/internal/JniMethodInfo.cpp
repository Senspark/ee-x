//
//  EEJniMethodInfo.cpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#include "ee/core/internal/JniMethodInfo.hpp"

namespace ee {
namespace core {
using Self = JniMethodInfo;

std::unique_ptr<Self> Self::create(JNIEnv* env, jclass clazz,
                                   jmethodID methodId) {
    return std::unique_ptr<Self>(new Self(env, clazz, methodId));
}

Self::JniMethodInfo(JNIEnv* env, jclass clazz, jmethodID methodId)
    : env_(env)
    , clazz_(clazz)
    , methodId_(methodId) {}

Self::~JniMethodInfo() {
    if (env_ != nullptr && clazz_ != nullptr) {
        // Release resources.
        env_->DeleteLocalRef(clazz_);
    }
}
} // namespace core
} // namespace ee
