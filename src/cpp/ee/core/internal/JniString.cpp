//
//  JniString.cpp
//  ee-core
//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/core/internal/JniString.hpp"

namespace ee {
namespace core {
std::unique_ptr<JniString> JniString::create(JNIEnv* env,
                                             const std::u16string& str) {
    return std::unique_ptr<JniString>(new JniString(env, str));
}

JniString::JniString(JNIEnv* env, const std::u16string& str)
    : env_(env)
    , javaString_(nullptr) {
    javaString_ = env_->NewString(reinterpret_cast<const jchar*>(str.c_str()),
                                  str.size());
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
