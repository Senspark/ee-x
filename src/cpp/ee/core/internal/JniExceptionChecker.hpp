//
//  JniExceptionChecker.hpp
//  Pods
//
//  Created by eps on 7/7/20.
//

#ifndef EE_X_JNI_EXCEPTION_CHECKER_HPP
#define EE_X_JNI_EXCEPTION_CHECKER_HPP

#include <jni.h>

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
/// Checks and print any JNI exception.
class JniExceptionChecker final {
public:
    explicit JniExceptionChecker(JNIEnv* env);
    ~JniExceptionChecker();

private:
    JNIEnv* env_;
};
} // namespace core
} // namespace ee

#endif /* EE_X_JNI_EXCEPTION_CHECKER_HPP */
