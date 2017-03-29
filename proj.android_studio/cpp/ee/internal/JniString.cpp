//
//  JniString.cpp
//  ee-core
//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/internal/JniString.hpp"

namespace ee {
namespace core {
std::unique_ptr<JniString> JniString::create(JNIEnv* env, const char* str) {
    return std::unique_ptr<JniString>(new JniString(env, str));
}

JniString::JniString(JNIEnv* env, const char* str)
    : env_{env}
    , javaString_{nullptr} {
    if (str != nullptr) {
        javaString_ = env_->NewStringUTF(str);
    }
}

JniString::~JniString() {
    if (javaString_ != nullptr) {
        // Release resources.
        env_->DeleteLocalRef(javaString_);
    }
}

jstring JniString::get() const noexcept {
    return javaString_;
}
} // namespace core
} // namespace ee
