//
//  JniExceptionChecker.cpp
//  Pods
//
//  Created by eps on 7/7/20.
//

#include "ee/core/internal/JniExceptionChecker.hpp"

namespace ee {
namespace core {
using Self = JniExceptionChecker;

Self::JniExceptionChecker(JNIEnv* env)
    : env_(env) {}

Self::~JniExceptionChecker() {
    if (env_->ExceptionCheck()) {
        env_->ExceptionDescribe();
        env_->ExceptionClear();
    }
}
} // namespace core
} // namespace ee
