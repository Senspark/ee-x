//
//  JniString.hpp
//  ee-core
//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_CORE_JNI_STRING_HPP_
#define EE_CORE_JNI_STRING_HPP_

#include <memory>

#include <jni.h>

namespace ee {
/// RAII class for encapsulating jstring.
class JniString {
public:
    static std::unique_ptr<JniString> create(JNIEnv* env, const char* str);

    ~JniString();

    jstring get() const noexcept;

private:
    explicit JniString(JNIEnv* env, const char* str);

    /// Weak pointer to the JNIEnv.
    JNIEnv* env_;

    /// Strong pointer the jstring.
    jstring javaString_;
};
} // namespace ee

#endif /* EE_CORE_JNI_STRING_HPP_ */
